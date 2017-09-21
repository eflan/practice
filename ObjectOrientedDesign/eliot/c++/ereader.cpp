#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
#include <unordered_map>

class EReaderException : public std::exception
{
public:
	EReaderException(const char *const msg)
		: _what(msg)
	{
	}
	
	EReaderException(const std::string &msg)
		: _what(msg)
	{
	}

	virtual const char *what() const throw()
	{
		return _what.c_str();
	}
	
private:
	std::string _what;
};

//
// Holds all the details of a book
//
class BookDetails
{
public:
	BookDetails()
		: _title(),
		  _authors(),
		  _editors(),
		  _publisher(),
		  _published(),
		  _keywords()
	{
	}
	
	BookDetails(const std::string &bookTitle,
				const std::vector<std::string> &bookAuthors,
				const std::vector<std::string> &bookEditors,
				const std::string &bookPublisher,
				const tm &bookPublished,
				const std::vector<std::string> &bookKeywords)
		: _title(bookTitle),
		  _authors(bookAuthors),
		  _editors(bookEditors),
		  _publisher(bookPublisher),
		  _published(bookPublished),
		  _keywords(bookKeywords)
	{
	}
		
	const std::string &title() const
	{
		return _title;
	}
	
	const std::vector<std::string> &authors() const
	{
		return _authors;
	}
	
	const std::vector<std::string> &editors() const
	{
		return _editors;
	}
	
	const std::string &publisher() const
	{
		return _publisher;
	}
	
	const tm &publicationDate() const
	{
		return _published;
	}
	
	const std::vector<std::string> &keywords() const
	{
		return _keywords;
	}
	
protected:
	void copyDetails(const BookDetails &copy)
	{
		_title = copy._title;
		_authors = copy._authors;
		_editors = copy._editors;
		_publisher = copy._publisher;
		_published = copy._published;
		_keywords = copy._keywords;
	}

	std::string _title;
	std::vector<std::string> _authors;
	std::vector<std::string> _editors;
	std::string _publisher;
	tm _published;
	std::vector<std::string> _keywords;
};

//
// Interface that defines all interactions with a Book
//
class Book : public BookDetails
{
public:
	Book() : BookDetails() {}
	
	Book(const std::string &bookTitle,
	 	 const std::vector<std::string> &bookAuthors,
		 const std::vector<std::string> &bookEditors,
		 const std::string &bookPublisher,
		 const tm &bookPublished,
		 const std::vector<std::string> &bookKeywords)
		: BookDetails(bookTitle,
					  bookAuthors,
					  bookEditors,
					  bookPublisher,
					  bookPublished,
					  bookKeywords)
	{
	}

	Book(const Book &copy)
		: BookDetails(copy.title(),
					  copy.authors(),
					  copy.editors(),
					  copy.publisher(),
					  copy.publicationDate(),
					  copy.keywords())
	{
	}
	
	virtual const unsigned int numPages() const = 0;
	virtual const std::string getPage(const unsigned int pageNumber) const = 0;
};

typedef uint64_t UserID;

class UserProfile
{
public:
	virtual const UserID ID() const = 0;	
	virtual const std::string &name() const = 0;	
};

class AuthenticationServer
{
public:	
	AuthenticationServer()
		: _nextID(1),
		  _usersByID(),
		  _nameToID()
	{
	}
	
	AuthenticationServer(std::unordered_map<UserID, std::string> users)
		: _nextID(0),
		  _usersByID(),
		  _nameToID()
	{
		//
		// Iterate over the map, creating the _nameToID map
		// and the usersByID map while also keeping track
		// of the largest ID seen.
		//
		
		UserID biggestID = 0;
		for(std::pair<const UserID, std::string> &user : users)
		{
			_nameToID[user.second] = user.first;
			_usersByID[user.first] = LiveUserProfile(user.first, user.second);
			
			if(user.first > biggestID)
			{
				biggestID = user.first;
			}
		}
		
		//
		// If this overflows we have more users than we have bytes of addressable memory
		// as well as 23 billion times more users than Earth's population in 2015.
		//
		_nextID = biggestID + 1;
	}
	
	const UserID registerNewUser(const std::string &name)
	{
		try
		{
			auto it = _nameToID.find(name);
			if(it == _nameToID.end())
			{
				const UserID id = nextID();
				_nameToID[name] = id;
				_usersByID[id] = LiveUserProfile(id, name);
				return id;
			}
			else
			{
				throw UserExistsException(_usersByID.at(it->second));
			}
		}
		catch(std::out_of_range &notFound)
		{
			throw DatabaseCorruptionException();
		}
	}
	
	const UserProfile &signin(const std::string &user)
	{
		try
		{
			LiveUserProfile &profile = _usersByID.at(_nameToID.at(user));
			profile.signin();
			return profile;
		}
		catch(const std::out_of_range &notFound)
		{
			throw UserDoesNotExistException(user);
		}
	}
	
	void signout(const UserProfile &profile)
	{
		try
		{
			_usersByID.at(profile.ID()).signout();
		}
		catch(const std::out_of_range &notFound)
		{
			throw UserDoesNotExistException(profile.name());
		}
	}
	
	bool isSignedIn(const UserProfile &profile) const
	{
		try
		{
			return _usersByID.at(profile.ID()).isSignedIn();
		}
		catch(const std::out_of_range &notFound)
		{
			throw UserDoesNotExistException(profile.name());
		}
	}
	
	bool isRegistered(const UserProfile &profile) const
	{
		auto it = _usersByID.find(profile.ID());
		return it != _usersByID.end();
	}

private:
	static std::string tmToString(const tm &time)
	{
		static const char wday_name[][4] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
		static const char mon_name[][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
										    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
										    
		char timeBuffer[26];
		sprintf(timeBuffer, "%.3s %.3s%3d %.2d:%.2d:%.2d %d",
				wday_name[time.tm_wday],
				mon_name[time.tm_mon],
				time.tm_mday,
				time.tm_hour,
				time.tm_min,
				time.tm_sec,
				1900 + time.tm_year);
				
		std::string timeString(timeBuffer);
		return timeString;
	}
	
	class LiveUserProfile : public UserProfile
	{
	public:
		LiveUserProfile(const UserID id = 0, const std::string &userName = std::string())
			: UserProfile(),
			  _ID(id),
			  _name(userName),
			  _signedIn(false),
			  _signinTime(),
			  _signoutTime()
		{
		}
		
		virtual const UserID ID() const
		{
			return _ID;
		}
		
		virtual const std::string &name() const
		{
			return _name;
		}
		
		bool isSignedIn() const
		{
			return _signedIn;
		}
		
		void signin()
		{
			if(_signedIn)
			{
				throw UserAlreadySignedInException(*this);
			}
			else
			{
				time_t rawTime = {};
				time(&rawTime);
				// there is a race here due to C stdlib using a static tm structure
				_signinTime = *gmtime(&rawTime);
				_signedIn = true;
			}
		}
		
		void signout()
		{
			if(!_signedIn)
			{
				throw UserAlreadySignedOutException(*this);
			}
			else
			{
				time_t rawTime = {};
				time(&rawTime);
				// there is a race here due to C stdlib using a static tm structure
				_signoutTime = *gmtime(&rawTime);
				_signedIn = false;
			}
		}
		
		const tm &signinTime() const
		{
			return _signinTime;
		}
		
		const tm &signoutTime() const 
		{
			return _signoutTime;
		}
		
	private:
		UserID _ID;
		std::string _name;
		bool _signedIn;
		tm _signinTime;
		tm _signoutTime;
	};
	
	class UserException : public EReaderException
	{
	public:
		UserException(const char *const message,
		              const UserProfile &profile)
			: EReaderException(getBasicDescription(message, profile.ID(), profile.name()))
		{
		}
		
		UserException(const char *const message,
		              const std::string &name)
			: EReaderException(getBasicDescription(message, name, " "))
		{
		}
		
		UserException(const std::string description)
			: EReaderException(description)
		{
		}
		
	protected:
		const std::string getBasicDescription(const char *const message,
											  const UserID id,
											  const std::string &name)
		{
			char idBuffer[32] = {};
			sprintf(idBuffer, " (ID: %lu) ", id);
									
			return getBasicDescription(message, name, idBuffer);
		}

		const std::string getBasicDescription(const char *const message,
											  const std::string &name,
											  const char *const afterName)
		{
			std::string description("User ");			
			
			description += name;
			description += afterName;
			description += message;
									
			return description;
		}
	};
	
	class LiveUserException : public UserException
	{
	public:
		LiveUserException(const char *const message,
		                  const LiveUserProfile &profile,
		                  const tm &eventTime)
			: UserException(getLiveDescription(message, profile, eventTime))
		{
		}
		
	private:		
		const std::string getLiveDescription(const char *const message,
		                                     const LiveUserProfile &profile,
		                                     const tm &eventTime)
		{
			std::string description = getBasicDescription(message, profile.ID(), profile.name());			
			description += tmToString(eventTime);			
			return description;
		}
	};

	class UserAlreadySignedInException : public LiveUserException
	{
	public:
		UserAlreadySignedInException(const LiveUserProfile &profile)
			: LiveUserException(" is already signed in as of ", profile, profile.signinTime())
		{
		}
	};

	class UserAlreadySignedOutException : public LiveUserException
	{
	public:
		UserAlreadySignedOutException(const LiveUserProfile &profile)
			: LiveUserException(" is already signed out as of ", profile, profile.signoutTime())
		{
		}
	};
	
	class UserExistsException : public UserException
	{
	public:
		UserExistsException(const UserProfile &profile)
			: UserException("already exists.", profile.name())
		{
		}
	};

	class UserDoesNotExistException : public UserException
	{
	public:
		UserDoesNotExistException(const std::string &name)
			: UserException("does not exist.", name)
		{
		}
	};
	
	class DatabaseCorruptionException : public EReaderException
	{
	public:
		DatabaseCorruptionException() : EReaderException("User database is corrupted!") {}
	};
	
	const UserID nextID()
	{
		return _nextID++;
	}
	
	UserID _nextID;
	std::unordered_map<UserID, LiveUserProfile> _usersByID;
	std::unordered_map<std::string, UserID> _nameToID;
};

class BookInvalidatedException : public EReaderException
{
public:
	BookInvalidatedException() : EReaderException("Book is no longer valid due to Library going offline.") {}
};

class InvalidBookException : public EReaderException
{
public:
	InvalidBookException() : EReaderException("Book is invalid") {}
};

class BookNotInLibraryException : public EReaderException
{
public:
	BookNotInLibraryException(const size_t index) : EReaderException(getDescription(index)) {}
	
private:
	static std::string getDescription(const size_t index)
	{
		char buffer[100];
		sprintf(buffer, "Book with index %zu is not in this libary.", index);
		std::string description(buffer);
		return description;
	}
};

class BookCheckedOutException : public EReaderException
{
public:
	BookCheckedOutException(const Book &book, const UserID user) : EReaderException(getDescription(book, user)) {}

private:
	static std::string getDescription(const Book &book, const UserID user)
	{
		char buffer[200];
		sprintf(buffer, "User with ID %zu has already checked out \"%s\".", user, book.title().c_str());
		return std::string(buffer);
	}
};

class BookNotCheckedOutException : public EReaderException
{
public:
	BookNotCheckedOutException(const Book &book) : EReaderException(getDescription(book)) {}

private:
	static std::string getDescription(const Book &book)
	{
		char buffer[200];
		sprintf(buffer, "\"%s\" is not checked out.", book.title().c_str());
		return std::string(buffer);
	}
};

typedef std::unordered_multimap<std::string, size_t> StringIndexMap;

class Library
{
public:
	Library(std::vector<Book *> &books)
	{
		buildIndices(books);
	}

	//
	// N.B. Library must exist so longs as any outstanding references
	// to LibraryBooks exist. Going offline invalidates all outstanding
	// references.
	//
	void offline()
	{
		for(LibraryBook &book : _books)
		{
			book.invalidate();
		}
	}

	const std::vector<size_t> byTitle(const std::string &title) const
	{
		return bookRangeToVector(_booksByTitle, title);
	}

	const std::vector<size_t> byAuthor(const std::string &author) const
	{
		return bookRangeToVector(_booksByAuthor, author);
	}
	
	const std::vector<size_t> byEditor(const std::string &editor) const
	{
		return bookRangeToVector(_booksByEditor, editor);
	}
	
	const std::vector<size_t> byPublisher(const std::string &publisher) const
	{
		return bookRangeToVector(_booksByEditor, publisher);
	}
	
	const std::vector<size_t> byKeyword(const std::string &editor) const
	{
		return bookRangeToVector(_booksByEditor, editor);
	}
	
	bool isCheckedOut(const size_t index, UserID *who) const
	{
		const LibraryBook &libraryBook = findLibraryBook(index);
		if(libraryBook.isCheckedOut())
		{
			*who = libraryBook.checkedOutBy();
			return true;
		}
		else
		{
			return false;
		}
	}
	
	const size_t countBooks() const
	{
		return _books.size();
	}
	
	const BookDetails getBookDetails(const size_t index) const
	{
		return _books[index];
	}
	
	const Book &checkout(const UserProfile &profile, const size_t index)
	{
		LibraryBook &libraryBook = findLibraryBook(index);
		libraryBook.checkout(profile);
		return libraryBook;
	}

	void returnBook(size_t index)
	{
		findLibraryBook(index).returnBook();
	}
	
private:
	class LibraryBook : public Book
	{
	public:
		LibraryBook()
			: Book(),
			  _book(nullptr),
			  _checkedOut(false),
			  _checkedOutBy(0)
		{
		}

		LibraryBook(Book *book)
			: Book(*book),
			  _book(book),
			  _checkedOut(false),
			  _checkedOutBy(0)
		{
		}
		
		LibraryBook(const LibraryBook &copy)
			: Book(copy),
			  _book(copy._book),
			  _checkedOut(copy._checkedOut),
			  _checkedOutBy(copy._checkedOutBy)
		{
		}
		
		LibraryBook &operator=(const LibraryBook &copy)
		{
			copyDetails(copy);
			_book = copy._book;
			_checkedOut = copy._checkedOut;
			_checkedOutBy = copy._checkedOutBy;
			return *this;
		}
		
		//
		// Book methods
		//
		
		virtual const unsigned int numPages() const
		{
			checkValid();
			return _book->numPages();
		}
		
		virtual const std::string getPage(const unsigned int pageNumber) const
		{
			checkValid();
			return _book->getPage(pageNumber);
		}

		//
		// LibraryBook methods
		//
		
		void invalidate()
		{
			_book = nullptr;
		}
		
		const bool isCheckedOut() const
		{
			return _checkedOut;
		}
		
		const UserID checkedOutBy() const
		{
			return _checkedOutBy;
		}
		
		void checkout(const UserProfile &profile)
		{
			if(_checkedOut)
			{
				throw BookCheckedOutException(*this, _checkedOutBy);
			}
			else
			{
				_checkedOut = true;
				_checkedOutBy = profile.ID();
			}
		}
		
		void returnBook()
		{
			if(!_checkedOut)
			{
				throw BookNotCheckedOutException(*this);
			}
			else
			{
				_checkedOut = false;
				_checkedOutBy = 0;
			}
		}
		
	private:
		void checkValid() const
		{
			if(_book == nullptr)
			{
				throw BookInvalidatedException();
			}
		}
		
		Book *_book;
		bool _checkedOut;
		UserID _checkedOutBy;
	};
	
	void buildIndices(std::vector<Book *> &books)
	{
		for(Book *book : books)
		{
			_books.push_back(LibraryBook(book));
			const size_t index = _books.size() - 1;
			LibraryBook &libraryBook = _books[index];
			
			std::pair<const std::string, size_t> insertTitle(libraryBook.title(), index);
			_booksByTitle.insert(insertTitle);

			for(const std::string &author : libraryBook.authors())
			{
				std::pair<const std::string, size_t> insertMe(author, index);
				_booksByAuthor.insert(insertMe);
			}

			for(const std::string &editor : libraryBook.editors())
			{
				std::pair<const std::string, size_t> insertMe(editor, index);
				_booksByEditor.insert(insertMe);
			}

			std::pair<const std::string, size_t> insertPublisher(libraryBook.publisher(), index);
			_booksByPublisher.insert(insertPublisher);

			for(const std::string &keyword : libraryBook.keywords())
			{
				std::pair<const std::string, size_t> insertMe(keyword, index);
				_booksByKeyword.insert(insertMe);
			}
		}
	}
	
	static const std::vector<size_t> bookRangeToVector(const StringIndexMap &multimap, const std::string &key)
	{
		std::vector<size_t> books;
		auto range = multimap.equal_range(key);
		auto end = range.second;
		
		for(auto it = range.first; it != end; it++)
		{
			books.push_back(it->second);
		}
		
		return books;
	}
	
	const LibraryBook &findLibraryBook(size_t index) const
	{
		if(index < _books.size())
		{
			return _books[index];
		}
		else
		{
			throw BookNotInLibraryException(index);
		}
	}

	LibraryBook &findLibraryBook(size_t index)
	{			    
		if(index < _books.size())
		{
			return _books[index];
		}
		else
		{
			throw BookNotInLibraryException(index);
		}
	}
	
	std::vector<LibraryBook> _books;
	StringIndexMap _booksByTitle;
	StringIndexMap _booksByAuthor;
	StringIndexMap _booksByEditor;
	StringIndexMap _booksByPublisher;
	StringIndexMap _booksByKeyword;
};

class TestUserProfile : public UserProfile
{
public:
	TestUserProfile(const UserID id, const char *const name)
		: _ID(id),
		  _name(name)
	{
	}
	
	virtual const UserID ID() const
	{
		return _ID;
	}
	
	virtual const std::string &name() const
	{
		return _name;
	}

private:
	UserID _ID;
	std::string _name;
};

class TestBook : public Book
{
public:
	TestBook()
		: Book(randomTitle(),
		       randomAuthors(),
			   randomEditors(),
			   randomPublisher(),
			   randomDate(),
			   randomKeyWords()),
		  _numPages(randomNumPages())
	{
	}
	
	virtual const unsigned int numPages() const
	{
		return _numPages;
	}
	
	virtual const std::string getPage(const unsigned int pageNumber) const
	{
		if(pageNumber >= numPages())
		{
			char buffer[100];
			sprintf(buffer, "Page #%.4u is greater than book length of %.4u", pageNumber, numPages());
			throw std::out_of_range(std::string(buffer));
		}
		else
		{
			char pageBuffer[11];
			sprintf(pageBuffer, "Page #%.4u", pageNumber);
			std::string page(pageBuffer);
			return page;
		}
	}

private:
	static std::string randomTitle()
	{
		return randomString("Title");
	}
	
	static std::vector<std::string> randomAuthors()
	{
		return randomStrings(10, "Author");
	}

	static std::vector<std::string> randomEditors()
	{
		return randomStrings(4, "Editor");
	}

	static std::string randomPublisher()
	{
		return randomString("Publisher");
	}
	
	static tm randomDate()
	{
		tm utc = {};
		utc.tm_mday = rand() % 31;
		utc.tm_mon = rand() % 12;
		utc.tm_year = rand() % 116;
		utc.tm_wday = rand() % 7;
		utc.tm_yday = rand() % 366;
		utc.tm_isdst = 0;
		return utc;
	}
	
	static std::vector<std::string> randomKeyWords()
	{
		return randomStrings(25, "Keyword");
	}
	
	static const unsigned int randomNumPages()
	{
		return (rand() % 9999) + 1;
	}

	static std::vector<std::string> randomStrings(const unsigned int maxSize, const char *const prefix)
	{
		std::vector<std::string> collection;
		
		const unsigned int numStrings = (rand() % (maxSize - 1)) + 1;
		for(unsigned int i = 0; i < numStrings; i++)
		{
			collection.push_back(randomString(prefix));
		}
		
		return collection;
	}
	
	static std::string randomString(const char *const prefix)
	{
		char buffer[20];
		sprintf(buffer, "%s %.4u", prefix, rand() % 10000);
		return std::string(buffer);
	}
	
	unsigned int _numPages;
};

void printDetailsList(const std::vector<std::string> &list, const char endChar)
{
	char trailing = ',';
	
	for(size_t i = 0; i < list.size(); i++)
	{
		if(i == list.size() - 1)
		{
			trailing = endChar;
		}
		
		printf("%s%c ", list[i].c_str(), trailing);
	}

}
void printDetails(const BookDetails details)
{
	printDetailsList(details.authors(), '.');	
	printf("%s. ", details.title().c_str());
	printDetailsList(details.editors(), ',');
	printf("%u.", 1900 + details.publicationDate().tm_year);
	printf("\n");
}

void SimulateEReader()
{
	srand(time(NULL));
	AuthenticationServer authServer;

	std::string aliceName = std::string("Alice");
	std::string bobName = std::string("Bob");
	std::string malloryName = std::string("Mallory");
	std::string doesNotExistName = std::string("Does not exist");
	
	const UserID aliceUserID = authServer.registerNewUser(aliceName);
	printf("Registered %s with user ID %lu\n", aliceName.c_str(), aliceUserID);
	const UserID bobUserID = authServer.registerNewUser(bobName);
	printf("Registered %s with user ID %lu\n", bobName.c_str(), bobUserID);
	const UserID malloryUserID = authServer.registerNewUser(malloryName);
	printf("Registered %s with user ID %lu\n", malloryName.c_str(), malloryUserID);
	
	try
	{
		authServer.registerNewUser(malloryName);
	}
	catch(const EReaderException &e)
	{
		printf("Result of registering %s twice -- %s\n", malloryName.c_str(), e.what());
	}
	
	const UserProfile &aliceUser = authServer.signin(aliceName);
	const UserProfile &bobUser = authServer.signin(bobName);
	const UserProfile &malloryUser = authServer.signin(malloryName);

	if(!authServer.isSignedIn(aliceUser))
	{
		printf("User %s should be signed in!\n", aliceUser.name().c_str());
	}
	if(!authServer.isSignedIn(bobUser))
	{
		printf("User %s should be signed in!\n", bobUser.name().c_str());
	}
	if(!authServer.isSignedIn(malloryUser))
	{
		printf("User %s should be signed in!\n", malloryUser.name().c_str());
	}
	
	try
	{
		authServer.signin(malloryName);
	}
	catch(const EReaderException &e)
	{
		printf("Result of trying to sign %s in twice -- %s\n", malloryName.c_str(), e.what());
	}

	try
	{
		authServer.signin(doesNotExistName);
	}
	catch(const EReaderException &e)
	{
		printf("Result of trying to sign %s in twice -- %s\n", doesNotExistName.c_str(), e.what());
	}
	
	authServer.signout(aliceUser);
	authServer.signout(bobUser);
	authServer.signout(malloryUser);
	
	if(authServer.isSignedIn(aliceUser))
	{
		printf("User %s should be signed out!\n", aliceUser.name().c_str());
	}
	if(authServer.isSignedIn(bobUser))
	{
		printf("User %s should be signed out!\n", bobUser.name().c_str());
	}
	if(authServer.isSignedIn(malloryUser))
	{
		printf("User %s should be signed out!\n", malloryUser.name().c_str());
	}
	
	try
	{
		authServer.signout(malloryUser);
	}
	catch(const EReaderException &e)
	{
		printf("Result of trying to sign %s out twice -- %s\n", malloryUser.name().c_str(), e.what());
	}

	TestUserProfile nonexistentProfile(0, "Nonexistent");	
	
	try
	{
		authServer.signout(nonexistentProfile);
	}
	catch(const EReaderException &e)
	{
		printf("Result of trying to sign %s out -- %s\n", nonexistentProfile.name().c_str(), e.what());
	}
	
	std::unordered_map<UserID, std::string> registeredUsers;
	
	char userNameBuffer[10];
	for(unsigned int i = 0; i < 1000; i++)
	{
		sprintf(userNameBuffer, "User %u", i);
		std::string userName(userNameBuffer);
		registeredUsers[i] = userName;
	}
	
	AuthenticationServer duplicateAuthServer(registeredUsers);
	
	for(std::pair<const UserID, std::string> &user : registeredUsers)
	{
		const UserProfile &profile = duplicateAuthServer.signin(user.second);
		
		if(!duplicateAuthServer.isSignedIn(profile))
		{
			printf("User %s should be signed in!\n", profile.name().c_str());
		}
		
		duplicateAuthServer.signout(profile);
		
		if(duplicateAuthServer.isSignedIn(profile))
		{
			printf("User %s should be signed out!\n", profile.name().c_str());
		}		
	}
	
	std::vector<TestBook> testBooks(10000);
	std::vector<Book *> books;
	
	for(TestBook &testBook : testBooks)
	{
		books.push_back(&testBook);
	}
	
	Library library(books);
	
	const size_t countBooks = library.countBooks();
	printf("Random book #1\n");
	printDetails(library.getBookDetails(rand() % countBooks));
	printf("Random book #2\n");
	printDetails(library.getBookDetails(rand() % countBooks));
	printf("Random book #3\n");
	printDetails(library.getBookDetails(rand() % countBooks));
	
	const std::vector<size_t> byAuthor = library.byAuthor(testBooks[0].authors()[0]);
	for(const size_t index : byAuthor)
	{
		const Book &book = library.checkout(aliceUser, index);
		printf("%s checked out \"%s\"\n", aliceUser.name().c_str(), book.title().c_str());
		
		try
		{
			const Book &shouldFail = library.checkout(malloryUser, index);
			printf("Checking out \"%s\" should have failed!\n", shouldFail.title().c_str());
		}
		catch(const BookCheckedOutException &)
		{
		}		
	}
	
	for(const size_t index : byAuthor)
	{
		library.returnBook(index);
		
		try
		{
			library.returnBook(index);
			printf("Returning book at index %zu should have failed!\n", index);
		}
		catch(const BookNotCheckedOutException &)
		{
		}
		
		const Book &book = library.checkout(bobUser, index);
		printf("%s checked out \"%s\"\n", bobUser.name().c_str(), book.title().c_str());
	}
}
