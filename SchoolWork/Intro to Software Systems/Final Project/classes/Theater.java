package classes;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

public class Theater {

	private Map<String, Movie> movies;
	private Map<String, Account> users;
	private ArrayList<Ticket> allTickets;
	private String managerPass = "password";

	public Theater() {
		this.movies = new HashMap<String, Movie>();
		this.users = new HashMap<String, Account>();
		this.allTickets = new ArrayList<>();
		//Scanner for starting movie and user info
		try {
			Scanner m = new Scanner(new File("movieInfo.txt"));

			while (m.hasNextLine()) {
				String title = m.nextLine();
				if (title.equals("") && m.hasNextLine()) {
					title = m.nextLine();
				}
				String rating = m.nextLine().trim();
				int minutes = Integer.parseInt((m.nextLine().trim()));
				String description = m.nextLine();
				double price = Double.parseDouble(m.nextLine().trim());

				Movie tempMovie = new Movie(title, minutes, description, price, rating);

				Scanner showings = new Scanner(m.nextLine());

				String day = "";
				while (showings.hasNext()) {
					String temp = showings.next().trim();
					int time = 0;
					try {
						time = Integer.parseInt(temp);
					} catch (NumberFormatException e) {
						day = temp;
					}
					if (time != 0) {
						tempMovie.addShowtime(time, day);
					}
				}
				showings.close();
				this.movies.put(title, tempMovie);

			}
			m.close();

			Scanner u = new Scanner(new File("userInfo.txt"));
			while (u.hasNextLine()) {
				Scanner temp = new Scanner(u.nextLine());
				String uName = temp.next().trim();
				String pass = temp.next().trim();
				Account newUser = new Account(uName, pass);
				users.put(uName, newUser);
				temp.close();
			}
			u.close();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}

	}

	public void close() {
		//Write all information previously existing and newly entered to files.
		try {
			PrintWriter movieOut = new PrintWriter("movieInfo.txt");
			PrintWriter userOut = new PrintWriter("userInfo.txt");
			for (Map.Entry<String, Movie> mo : this.movies.entrySet()) {
				movieOut.print(mo.getValue().toString());
			}

			for (Map.Entry<String, Account> us : this.users.entrySet()) {
				userOut.println(us.getValue().toString());
			}
			movieOut.close();
			userOut.close();
		} catch (FileNotFoundException e) {

		}
	}

	public boolean managerView(Scanner in) {
		while (true) {
			System.out.println("What action would you like to take?");
			System.out.println("Add | Edit | Enter Customer View (back) | Close Application (close)");
			String input = in.next();
			if (input.equalsIgnoreCase("add")) {
				String title, description, day, rating;
				double price;
				int runTime, hour = 0;
				boolean done1 = false, done2 = false, contains = false;
				do {
					System.out.print("Please enter the title: ");
					title = in.next();
					for (Map.Entry<String, Movie> mo : this.movies.entrySet()) {
						if (!contains && mo.getKey().equals(title)) {
							contains = true;
						}
					}
				} while (contains);
				System.out.print("Please enter the description: ");
				description = in.next();
				System.out.print("Please enter the ticket price: ");
				price = in.nextDouble();
				System.out.print("Please enter the rating: ");
				rating = in.next();
				System.out.print("Please enter the runtime in minutes: ");
				runTime = in.nextInt();
				Movie newMovie = new Movie(title, runTime, description, price, rating);
				do {
					System.out.println("Please enter the day of the week for these showings");
					System.out.print("Day: ");
					day = in.next();
					while (!done1) {
						System.out.print("Hour, type done when there are no more times for this day (military time, no minutes): ");
						String temp = in.next();
						if (temp.equalsIgnoreCase("done")) {
							done1 = true;
						} else {
							try {
								hour = Integer.parseInt(temp);
								newMovie.addShowtime(hour, day);
							} catch (NumberFormatException e) {
								
							}
						}
					}
					
					System.out.print("Any more days? (yes / no)");
					String temp2 = in.next();
					if (temp2.equalsIgnoreCase("no")) {
						done2 = true;
					}
				} while (!done2);
			} else if (input.equalsIgnoreCase("edit")) {
				boolean check = false;
				String title = "";
				do {
					System.out.print("Please enter the title of the movie in question: ");
					title = in.next();
					for (Map.Entry<String, Movie> mo : this.movies.entrySet()) {
						if (!check) {
							check = mo.getKey().equals(title);
						}
					}
				} while (!check);
				do {
					System.out.println("What do you want to change?");
					System.out.println("Title (title) | description | ticket price (price) | runtime | rating | showtimes (hour) | done");
					String manageInput = in.next();
					if (manageInput.equalsIgnoreCase("title")) {
						
						System.out.print("Enter the new title: ");
						String newTitle = in.next();
						this.movies.get(title).setTitle(newTitle);
						
					} else if (manageInput.equalsIgnoreCase("description")) {
						
						System.out.print("Enter the new description: ");
						String newDesc = in.next();
						this.movies.get(title).setDescription(newDesc);
						
					} else if (manageInput.equalsIgnoreCase("price")) {
						
						System.out.print("Enter the new price: ");
						double newPrice = in.nextDouble();
						this.movies.get(title).setPrice(newPrice);
						
					} else if (manageInput.equalsIgnoreCase("runtime")) {
						
						System.out.print("Enter the new runtime: ");
						int newRuntime = in.nextInt();
						this.movies.get(title).setRunTime(newRuntime);
						
					} else if (manageInput.equalsIgnoreCase("rating")) {
						
						System.out.print("Enter the new rating: ");
						String newRating = in.next();
						this.movies.get(title).setRating(newRating);
						
					} else if (manageInput.equalsIgnoreCase("hour")) {
						System.out.println("What day: ");
						String newDay = in.next();
						System.out.println("What showing do you want to change?");
						this.movies.get(title).getShowtimes(newDay);
						System.out.println();
						int timeOne = Integer.parseInt(in.next().trim());
						System.out.println("What time would you like it to be?");
						int timeTwo = Integer.parseInt(in.next().trim());
						
						Showtime prev = new Showtime(timeOne, newDay);
						
						this.movies.get(title).setShowtime(prev, timeTwo);
						
						
					} else if (manageInput.equalsIgnoreCase("done")) {
						break;
					}
				} while (true);
			} else if (input.equalsIgnoreCase("back")) {
				return false;
			} else if (input.equalsIgnoreCase("close")) {
				return true;
			}
		}
	}

	public void movieView(Scanner in) {
		System.out.println("Would you like to see all showings for today or the week? (today / week)");
		String choice = in.next();
		if (choice.equalsIgnoreCase("today")) {
			for (Map.Entry<String, Movie> mo : this.movies.entrySet()) {
				System.out.println(mo.getValue().dayShows());
			}
		} else if (choice.equalsIgnoreCase("week")) {
			for (Map.Entry<String, Movie> mo : this.movies.entrySet()) {
				System.out.println(mo.getValue().weekShows());
			}
		}
	}

	public void paymentView(Scanner in) {
		ArrayList<Ticket> tickets = new ArrayList<>();
		System.out.println("How many tickets would you like to buy?");
		int ticketNumber = in.nextInt();
		for(int i = 0; i < ticketNumber; i++)
		{
			System.out.println(movies.keySet());
			System.out.println("What movie would you like to see?");
			String movieName = in.next();
			System.out.println("Are you purchasing for a child, adult or senior?");
			String ageType = in.next();
			System.out.println("Which day?");
			String day = in.next();
			System.out.println("Which of these times would you like to select");
			movies.get(movieName).getShowtimes(day);
			System.out.println();
			int hour = in.nextInt();
			
			System.out.println("\n");

			Showtime time = new Showtime(hour,day);
			Ticket tick = new Ticket(movies.get(movieName),ageType,time);
			tickets.add(tick);
		}
		System.out.println("What payment-type are you using (cash or credit)?");
		String paymentType = in.next();
		if(paymentType.equals("credit"))
		{
			System.out.println("Enter your Card Number");
			in.next(); //Card number not save cause we don't do anything with it
		}
		Payment order = new Payment(tickets,paymentType);
		for (Ticket t : tickets) {
			allTickets.add(t);
		}
		
		System.out.println(order.toString());
		System.out.println();
		System.out.println("Thanks for your order!");
	}

	public boolean login(String status, Scanner in) {
		int count = 0;
		if (status.equalsIgnoreCase("login")) {
			String uName;
			do {
				System.out.print("Please enter your username: ");
				uName = in.next().trim();
				if (!this.users.containsKey(uName)) {
					System.out.println("Username not found");
				}
			} while (!this.users.containsKey(uName));

			String pass = "";
			do {
				System.out.print("Please enter your password: ");
				pass = in.next().trim();
				if (users.get(uName).getPassword().equals(pass)) {
					System.out.println("Logged in");
					return true;
				} else {
					System.out.println("The password you have entered was incorrect. Please try again.");
					count++;
				}
			} while (!users.get(uName).getPassword().equals(pass) && count < 5);
		}

		if (status.equalsIgnoreCase("create") || count >= 5) {
			String confirm = "";
			String uName, pass;
			if (count >= 5) {
				System.out.println("You have entered the password wrong too many times. Please make an account.");
			}

			do {
				do {
					System.out.print("Please enter your username: ");
					uName = in.next().trim();
					if (this.users.containsKey(uName)) {
						System.out.println("Username already taken");
					}
				} while (this.users.containsKey(uName));
				System.out.print("Please enter your password: ");
				pass = in.next().trim();
				System.out.println("Are these correct? (yes / no)");
				confirm = in.next().trim();
				System.out.println("Logged in");
			} while(!confirm.equalsIgnoreCase("yes"));
			this.users.put(uName, new Account(uName, pass));
			return true;
		}
		return false;
	}

	public void customerView(Scanner input) {
		boolean loggedIn = false;
		do {
			System.out.print("Do you already have an accout? (login / create): ");
			loggedIn = login(input.next(), input);
		} while (!loggedIn);
		
		do {
			String line = "";
			for (int i = 0; i < 30; i++) {
				line = line + "_";
			}
			System.out.println(line);
			System.out.println("What action would you like to take?");
			System.out.println("Purchase tickets (purchase) "
							+ "| View the list of movies (movie) "
							+ "| View purchased tickets (tickets) "
							+ "| Manage movies (manage) "
							+ "| Close the application (close)");
			String action = input.next();
			if (action.equalsIgnoreCase("purchase")) {
				this.paymentView(input);
			} else if (action.equalsIgnoreCase("movie")) {
				movieView(input);
			} else if (action.equalsIgnoreCase("tickets")) {
				if (allTickets.size() == 0) {
					System.out.println("You have not puchased any tickets yet." + "\n");
				} else {
					for (Ticket t : allTickets) {
						System.out.println(t.toString());
					}
				}
			} else if (action.equalsIgnoreCase("manage")) {
				int attempts = 0;
				boolean correct = false;
				do {
					System.out.print("Input the manager password: ");
					String pass = input.next();
					attempts++;
					if (pass.equals(managerPass)) {
						correct = true;
					} else {
						System.out.println("Password incorrect");
					}
				} while (!correct && attempts < 3);
				
				if (correct) {
					boolean result = managerView(input);
					if (result) {
						break;
					}
				} else {
					break;
				}
			} else if (action.equalsIgnoreCase("close")) {
				break;
			}
		} while (true);
		close();
	}

	public static void main(String[] args) {
		Theater theater = new Theater();
		
		Scanner in = new Scanner(System.in);
		theater.customerView(in);
		
		//Console interface content here
		//Customer perspective goes here

	}
}
