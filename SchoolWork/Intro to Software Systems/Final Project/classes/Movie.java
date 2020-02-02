package classes;

import java.util.ArrayList;

public class Movie {
	
	private String title, description;
	private int runTime;
	private ArrayList<Showtime> showtimes;
	private String rating;
	private double price;

	public Movie(String title, int minutes, String description, double price, String rating) {
		this.title = title;
		this.runTime = minutes;
		this.description = description;
		this.showtimes = new ArrayList<>();
		this.price = price;
		this.rating = rating;
	}

	public void addShowtime(int hour, String day) 
	{
		showtimes.add(new Showtime(hour, day));
	}
	
	public void removeShowtime(Showtime st) 
	{
		showtimes.remove(st);
	}
	
	public void getShowtimes(String day)
	{
		
		for (Showtime t : showtimes) {
			if (t.getDay().equalsIgnoreCase(day)) {
				System.out.print(t.getHour() + " ");
			}
		}
		
	}
	
	public void setShowtime(Showtime s, int newHour) {
		
		for (Showtime t : this.showtimes) {
			if (t.getDay().equalsIgnoreCase(s.getDay()) && t.getHour() == s.getHour()) {
				t.setHour(newHour);
			}
		}
	}
	public ArrayList<Showtime> getShowings(){
		return this.showtimes;
	}

	public String getTitle() {
		return title;
	}

	public void setTitle(String title) {
		this.title = title;
	}

	public int getRunTime() {
		return runTime;
	}

	public void setRunTime(int minutes) {
		this.runTime = minutes;
	}
	
	public String getRating() {
		return rating;
	}

	public void setRating(String rating) {
		this.rating = rating;
	}
	
	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}
	
	public double getPrice() {
		return price;
	}

	public void setPrice(double price) {
		this.price = price;
	}
	
	public String toString() {
		String day = "";
		String times = "";
		for (Showtime t : showtimes) {
			String newDay = t.getDay();
			if (!day.equalsIgnoreCase(newDay)) {
				day = newDay;
				if (times.isEmpty()) {
					times = day + " " + t.getHour();
				} else {
					times = times + " " + day + " " + t.getHour();
				}
			} else {
				times = times + " " + t.getHour();
			}
		}
		return this.title + "\n" + this.rating + "\n" + this.runTime + "\n" + this.description + "\n" + this.price + "\n" + times + "\n";
	}
	
	public String dayShows() {
		String out = this.title + ": ";
		String first = "";
		if (showtimes.size() != 0) {
			first = showtimes.get(0).getDay();
		}
		int i = 0;
		while (i < showtimes.size() && showtimes.get(i).getDay().equalsIgnoreCase(first)) {
			out = out + showtimes.get(i).getTime() + ", ";
			i++;
		}
		return out.substring(0, out.length() - 2);
	}
	
	public String weekShows() {
		String out = this.title + ": ";
		String first = "";
		if (showtimes.size() != 0) {
			first = showtimes.get(0).getDay();
		}
		int i = 0;
		out = out + first + " ";
		while (showtimes.get(i).getDay().equalsIgnoreCase(first)) {
			out = out + showtimes.get(i).getTime() + ", ";
			i++;
		}
		String current = showtimes.get(i).getDay();
		out = out.substring(0, out.length() - 2) + " " + current + " ";
		while (i < showtimes.size()) {
			if (showtimes.get(i).getDay().equalsIgnoreCase(first)) {
				break;
			} else if (showtimes.get(i).getDay().equalsIgnoreCase(current)) {
				out = out + showtimes.get(i).getTime() + ", ";
				i++;
			} else {
				current = showtimes.get(i).getDay();
				if (out.length() > 2) {
					out = out.substring(0, out.length() - 2) + " | " + current + ": ";
				}
			}
		}
		return out;
	}
}
