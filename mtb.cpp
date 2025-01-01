#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

class User {
public:
    string email;
    string password;

    User(string email, string password) : email(email), password(password) {}
};

class Movie {
public:
    string name;
    string date;
    string time;
    int availableSeats;
    vector<vector<char>> seats;

    Movie(string name, string date, string time, int availableSeats)
        : name(name), date(date), time(time), availableSeats(availableSeats), seats(6, vector<char>(12, '*')) {}
};

class Ticket {
public:
    string email;
    string movieName;
    string date;
    string time;
    int row;
    int column;

    Ticket(string email, string movieName, string date, string time, int row, int column)
        : email(email), movieName(movieName), date(date), time(time), row(row), column(column) {}
};

class MovieTicketBookingSystem {
private:
    vector<User> users;
    vector<Movie> movies;
    vector<Ticket> tickets;
    string currentUser;

    void clearScreen() {
        system("CLS");
    }

    void centerText(const string& text) {
        cout << setw(40 + text.size() / 2) << text << endl;
    }

    void loadUsers() {
        ifstream file("users.txt");
        string email, password;
        while (file >> email >> password) {
            users.emplace_back(email, password);
        }
        file.close();
    }

    void saveUsers() {
        ofstream file("users.txt");
        for (const auto& user : users) {
            file << user.email << " " << user.password << endl;
        }
        file.close();
    }

void loadMovies() {
    ifstream file("movies.txt");
    if (!file) {
        cerr << "Error: Could not open movies.txt" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name, date, time;
        int availableSeats;

        // Read name (handles quotes for names with spaces)
        if (ss.peek() == '"') {
            ss.ignore(); // Ignore the opening quote
            getline(ss, name, '"'); // Read until the closing quote
        } else {
            ss >> name; // Handle single-word names
        }

        ss >> date >> time >> availableSeats;

        // Create a new movie object
        Movie movie(name, date, time, availableSeats);

        // Read seating chart
        for (int i = 0; i < 6; ++i) {
            string seatRow;
            if (getline(file, seatRow)) {
                for (int j = 0; j < seatRow.size() && j < 12; ++j) {
                    movie.seats[i][j] = seatRow[j];
                }
            }
        }
        movies.push_back(movie);
    }

    file.close();
}


    void saveMovies() {
    ofstream file("movies.txt");
    if (!file) {
        cerr << "Error: Could not open movies.txt" << endl;
        return;
    }

    for (const auto& movie : movies) {
        file << "\"" << movie.name << "\" " // Save name in quotes
             << movie.date << " "
             << movie.time << " "
             << movie.availableSeats << endl;

        for (const auto& row : movie.seats) {
            for (const auto& seat : row) {
                file << seat;
            }
            file << endl;
        }
    }

    file.close();
}


    void loadTickets() {
    ifstream file("tickets.txt");
    if (!file) {
        cerr << "Error: Could not open tickets.txt" << endl;
        return;
    }

    tickets.clear(); // Clear existing tickets to prevent duplication or old data issues

    string email, movieName, date, time;
    int row, column;

    while (file >> email) {
        file.ignore(); // Ignore whitespace
        getline(file, movieName, '\"'); // Handle movie names with spaces
        getline(file, movieName, '\"'); // Read actual movie name inside quotes
        file >> date >> time >> row >> column;

        tickets.emplace_back(email, movieName, date, time, row, column);
    }

    file.close();
}


    void saveTickets() {
    ofstream file("tickets.txt", ios::trunc); // Open file in truncate mode to overwrite old data
    if (!file) {
        cerr << "Error: Could not open tickets.txt" << endl;
        return;
    }

    for (const auto& ticket : tickets) {
        file << ticket.email << " \"" << ticket.movieName << "\" " 
             << ticket.date << " " << ticket.time << " " 
             << ticket.row << " " << ticket.column << endl;
    }

    file.close();
}


    void signup() {
        clearScreen();
        	centerText("\t\t\t******************************************");
            centerText("\t\t\t  SIGNUP    ");
            centerText("\t\t\t******************************************\n\n");
        string email, password;
        cout << "\t\tEnter email: ";
        cin >> email;
        cout << "\t\tEnter password: ";
        cin >> password;
        users.emplace_back(email, password);
        saveUsers();
        cout << "\n\t\tSignup successful!" << endl;
        system("pause");
    }

    bool login() {
        clearScreen();
        	centerText("\t\t\t******************************************");
            centerText("\t\t\t  LOGIN    ");
            centerText("\t\t\t******************************************\n\n");
        string email, password;
        cout << "\t\tEnter email: ";
        cin >> email;
        cout << "\t\tEnter password: ";
        cin >> password;
        for (const auto& user : users) {
            if (user.email == email && user.password == password) {
                currentUser = email;
                return true;
            }
        }
        cout << "Invalid credentials! Please try again." << endl;
        return login(); // Retry login directly
    }

    void loginOrSignup() {
        int choice;
        do {
            clearScreen();
            centerText("\t\t\t***********************************************************");
            centerText("\t\t\t\t  LOGIN/SIGNUP    ");
            centerText("\t\t\t***********************************************************\n\n");
cout <<
    "                  \t========================        \t\t==========================\n"
    "                  \t* (1)     LOGIN        *        \t\t* (2)      SIGNUP        *\n"
    "                  \t========================        \t\t==========================\n\n";


            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                if (login()) {
                    userPanel();
                }
                break;
            case 2:
                signup();
                break;
            default:
                cout << "Invalid choice!" << endl;
                system("pause");
            }
        } while (choice != 1);
    }


void adminPanel() {
    clearScreen();
    centerText("\t\t\t******************************************");
    centerText("\t\t\t        ADMIN LOGIN    ");
    centerText("\t\t\t******************************************\n\n");
    string username, password;
    cout << "\n\t\tEnter admin username: ";
    cin >> username;
    cout << "\n\t\tEnter admin password: ";
    cin >> password;

    if (username == "admin" && password == "admin123") {
        int choice;
        do {
            clearScreen();
            centerText("\t\t\t******************************************");
            centerText("\t\t\t        ADMIN PANEL    ");
            centerText("\t\t\t******************************************\n\n");
cout <<
    "                  ========================        ==========================        ======================\n"
    "                  * (1)    Add Movie     *        * (2)     View Users     *        * (3)  Delete Movie  *\n"
    "                  ========================        ==========================        ======================\n\n"
    "                                                ==============================\n"
    "                                                * (4)        Logout          *\n"
    "                                                ==============================\n\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    string name, date, time;
                    int seats;
                    clearScreen();
                    centerText("\t\t\t******************************************");
                    centerText("\t\t\t        ADD MOVIE    ");
                    centerText("\t\t\t******************************************\n\n");

                    cout << "\tEnter movie name: ";
                    cin.ignore(); // To clear any leftover newline in the buffer
                    getline(cin, name);

                    cout << "\n\tEnter show date (dd-mm-yyyy): ";
                    cin >> date;

                    cout << "\n\tEnter show time (hh:mm): ";
                    cin >> time;

                    cout << "\n\tEnter available seats: ";
                    cin >> seats;

                    movies.emplace_back(name, date, time, seats);
                    saveMovies();
                    cout << "\n\n\t\t\t\tMovie added successfully!" << endl;
                    system("pause");
                    break;
                }
                case 2:
                    clearScreen();
                    centerText("\t\t\t******************************************");
                    centerText("\t\t\t    REGISTERED USERS    ");
                    centerText("\t\t\t******************************************\n\n");
                    for (const auto& user : users) {
                        cout << "\tUser Email:\t" << user.email << endl << endl;
                    }
                    system("pause");
                    break;
                case 3: {
                    clearScreen();
                    centerText("\t\t\t******************************************");
                    centerText("\t\t\t        DELETE MOVIE    ");
                    centerText("\t\t\t******************************************\n\n");

                    cout << "Available Movies:\n\n";

                    // Display movies with indices
                    for (size_t i = 0; i < movies.size(); ++i) {
                        cout << "\n(" << i + 1 << ") " << movies[i].name
                             << " | Date: " << movies[i].date
                             << " | Time: " << movies[i].time
                             << " | Seats: " << movies[i].availableSeats << endl;
                    }

                    int deleteChoice;
                    cout << "\nEnter the movie number to delete: ";
                    cin >> deleteChoice;

                    if (deleteChoice > 0 && deleteChoice <= movies.size()) {
                        // Delete the selected movie
                        movies.erase(movies.begin() + (deleteChoice - 1));
                        saveMovies();
                        cout << "Movie deleted successfully!" << endl;
                    } else {
                        cout << "Invalid choice!" << endl;
                    }
                    system("pause");
                    break;
                }
                case 4:
                    return;
                default:
                    cout << "Invalid choice!" << endl;
                    system("pause");
            }
        } while (choice != 4);
    } else {
        cout << "Invalid admin credentials!" << endl;
        system("pause");
    }
}




    void displaySeatingChart(const Movie& movie) {
        centerText("Seating Chart\n");
        cout << "    ";
        for (int col = 1; col <= 12; ++col) {
            cout << setw(3) << col;
        }
        cout << endl;

        for (int row = 0; row < 6; ++row) {
            cout << "  " << static_cast<char>('A' + row) << " ";
            for (int col = 0; col < 12; ++col) {
                cout << setw(3) << movie.seats[row][col];
            }
            cout << endl;
        }
    }


	void bookTicket() {
		const int TICKET_PRICE = 600;
    	clearScreen();
    	centerText("\t\t\t******************************************");
    	centerText("\t\t\t      BOOK TICKET    ");
    	centerText("\t\t\t******************************************\n\n");

    	cout << "Available Movies:\n\n";
		cout << string(102, '-') << endl;
    	// Display table headers with separators
    	cout << setw(10) << left << " S.NO"
         	<< "|" << setw(30) << left << "MOVIE NAME"
         	<< "|" << setw(15) << left << "SHOW DATE"
         	<< "|" << setw(15) << left << "SHOW TIME"
         	<< "|" << setw(20) << left << "SEATS AVAILABLE"
         	<< "|" << setw(10) << left << "PRICE |" << endl;

    	// Add a separator line
    	cout << string(102, '-') << endl;

    	// Loop through movies and display them in a table with separators
    	for (size_t i = 0; i < movies.size(); ++i) {
        	cout << setw(10) << left << "(" + to_string(i + 1) + ")"
     			<< "|" << setw(30) << left << movies[i].name // No extra quotes here
     			<< "|" << setw(15) << left << movies[i].date
     			<< "|" << setw(15) << left << movies[i].time
     			<< "|" << setw(20) << left << movies[i].availableSeats
     			<< "|" << setw(10) << left << TICKET_PRICE << endl;
    	}
    	cout << endl;

    	int choice;
    	cout << "Enter the movie number to book: ";
    	cin >> choice;

    	if (choice > 0 && choice <= movies.size() && movies[choice - 1].availableSeats > 0) {
        	Movie& selectedMovie = movies[choice - 1];

        	int seatsToBook;
        	cout << "Enter the number of seats to book: ";
        	cin >> seatsToBook;

        	if (seatsToBook > selectedMovie.availableSeats) {
            	cout << "Not enough seats available!" << endl;
            	system("pause");
            	return;
        	}


           		// Proceed with seat booking
            	for (int i = 0; i < seatsToBook; ++i) {
               		clearScreen();
               		displaySeatingChart(selectedMovie);

               		char rowChar;
               		int column;
               		cout << "\nEnter row (A-F): ";
               		cin >> rowChar;
               		cout << "Enter column number (1-12): ";
               		cin >> column;

               		int row = rowChar - 'A';
               		if (row >= 0 && row < 6 && column >= 1 && column <= 12 && selectedMovie.seats[row][column - 1] == '*') {
                   		selectedMovie.seats[row][column - 1] = 'X';
                   		selectedMovie.availableSeats--;

                   		tickets.emplace_back(currentUser, selectedMovie.name, selectedMovie.date, selectedMovie.time, row + 1, column);
               		} else {
                   		cout << "Invalid seat selection or seat already booked!" << endl;
                   		--i; // Retry the same seat booking
               		}
           		}
           		//Calculate total price
           		int totalPrice = seatsToBook * TICKET_PRICE;

        	// Display payment prompt
        	cout << "The total price is: " << totalPrice << " PKR. Please pay in cash.\n\n";
        	int paymentAmount;
        	cout << "Enter the amount you are paying: ";
        	cin >> paymentAmount;

        	// Payment validation
        	if (paymentAmount < totalPrice) {
           		cout << "Not enough money! Booking canceled.\n";
           		system("pause");
           		return;
        	} else {
           		int change = paymentAmount - totalPrice;
           		cout << "Payment successful! ";
           		if (change > 0) {
               		cout << "Here is your change: " << change << " PKR.\n";
           		} else {
               		cout << "No change required.\n";
           		}
           		cout << "Enjoy the movie!\n\n";

           		saveMovies();
           		saveTickets();
           		cout << "Ticket(s) booked successfully!" << endl;
        	}
    	} else {
       		cout << "Invalid choice or no available seats!" << endl;
    	}
    	system("pause");
	}



   void viewTickets() {
    clearScreen();
    centerText("\t\t\t******************************************");
    centerText("\t\t\t         YOUR TICKETS    ");
    centerText("\t\t\t******************************************\n\n");

    bool found = false; // Track if any ticket is found for the current user
    for (const auto& ticket : tickets) {
        if (ticket.email == currentUser) {
            cout << "\nMovie: " << ticket.movieName 
                 << " | Date: " << ticket.date 
                 << " | Time: " << ticket.time 
                 << " | Seat: Row " << static_cast<char>('A' + ticket.row - 1) 
                 << " Column " << ticket.column << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "\nNo tickets found for your account." << endl;
    }

    system("pause");
}


    void cancelTicket() {
    clearScreen();
    centerText("\t\t\t******************************************");
    centerText("\t\t\t         CANCEL TICKET    ");
    centerText("\t\t\t******************************************\n\n");

    viewTickets();

    string movieName;
    cout << "\nEnter the movie name to cancel: ";
    cin.ignore();
    getline(cin, movieName);

    char rowChar;
    int column;
    cout << "\nEnter row (A-F): ";
    cin >> rowChar;
    cout << "\nEnter column (1-12): ";
    cin >> column;

    int row = rowChar - 'A';

    // Find the ticket in the vector
    auto it = remove_if(tickets.begin(), tickets.end(), [&](const Ticket& ticket) {
        return ticket.email == currentUser && 
               ticket.movieName == movieName && 
               ticket.row == row + 1 && 
               ticket.column == column;
    });

    if (it != tickets.end()) {
        tickets.erase(it, tickets.end());
        saveTickets(); // Save updated tickets

        // Update the movie's seating chart
        for (auto& movie : movies) {
            if (movie.name == movieName) {
                movie.seats[row][column - 1] = '*';
                movie.availableSeats++;
                break;
            }
        }

        saveMovies(); // Save updated movie details
        cout << "Ticket canceled successfully!" << endl;
    } else {
        cout << "Ticket not found!" << endl;
    }

    system("pause");
}


    void searchMovie() {
        clearScreen();
        	centerText("\t\t\t******************************************");
            centerText("\t\t\t      SEARCH MOVIE    ");
            centerText("\t\t\t******************************************\n\n");
        string searchQuery;
        cout << "Enter movie name: ";
        cin.ignore();
        getline(cin, searchQuery);
        transform(searchQuery.begin(), searchQuery.end(), searchQuery.begin(), ::tolower);

        bool found = false;
        for (const auto& movie : movies) {
            string movieNameLower = movie.name;
            transform(movieNameLower.begin(), movieNameLower.end(), movieNameLower.begin(), ::tolower);

            if (movieNameLower.find(searchQuery) != string::npos) {
                cout << "\nMovie: " << movie.name << " | Date: " << movie.date << " | Time: " << movie.time << " | Seats: " << movie.availableSeats << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "\nNo movies found matching the query!" << endl;
        }
        system("pause");
    }

    void userPanel() {
        int choice;
        do {
            clearScreen();
            	centerText("\t\t\t******************************************");
            	centerText("\t\t\t        USER PANEL    ");
            	centerText("\t\t\t******************************************\n\n");
cout <<
    "                  ========================        ==========================        ======================\n"
    "                  * (1)   Book Ticket    *        * (2)    View Tickets    *        * (3)  Cancel Ticket *\n"
    "                  ========================        ==========================        ======================\n\n"
    "                                   ========================        ==========================\n"
    "                                   * (4)  Search Movie    *        * (5)      Logout        *\n"
    "                                   ========================        ==========================\n\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                bookTicket();
                break;
            case 2:
                viewTickets();
                break;
            case 3:
                cancelTicket();
                break;
            case 4:
                searchMovie();
                break;
            case 5:
                currentUser = "";
                return;
            default:
                cout << "Invalid choice!" << endl;
            }
        } while (choice != 5);
    }

public:
    MovieTicketBookingSystem() {
        loadUsers();
        loadMovies();
        loadTickets();
    }

    void run() {
        int choice;
        do {
            clearScreen();
            centerText("\t\t\t***********************************************************");
            centerText("\t\t\t\t  MOVIE TICKET BOOKING SYSTEM    ");
            centerText("\t\t\t***********************************************************\n\n");
cout <<
    "                  ========================        ==========================        ======================\n"
    "                  * (1)  LOGIN/SIGNUP    *        * (2)       ADMIN        *        * (3)      QUIT      *\n"
    "                  ========================        ==========================        ======================\n\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1:
                loginOrSignup();
                break;
            case 2:
                adminPanel();
                break;
            case 3:
                cout << "Thank you for using the system!" << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
            }
        } while (choice != 3);
    }
};

int main() {
    MovieTicketBookingSystem system;
    system.run();
    return 0;
}
