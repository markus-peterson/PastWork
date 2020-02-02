package classes;

public class Ticket {
	
	private String ageType;
	private Movie movie;
	private Showtime time;
	private double price;
	private boolean purchasable;
	

	public Ticket(Movie movie, String ageType, Showtime time) {
		this.movie = movie;
		this.ageType = ageType;
		this.time = time;
	}

	public String getAgeType() {
		return ageType;
	}

	public void setAgeType(String ageType) {
		this.ageType = ageType;
	}

	public Movie getMovie() {
		return movie;
	}

	public void setMovie(Movie movie) {
		this.movie = movie;
	}
	
	public Showtime getTime() {
		return time;
	}
	
	public void setPrice(double price) {
		this.price = price;
	}

	public boolean isPurchasable()
	{
		if(movie.getRating().equalsIgnoreCase("R")&& ageType.equalsIgnoreCase("child"))
		{
			purchasable = false;
		}
		else
		{
			purchasable = true;
		}
		return purchasable;
	}
	
	public double getPrice() 
	{ //Calculate ticket price based on variables
		
		if(ageType.equals("child"))
		{
			if(time.getTimeType().equals("primetime"))
			{
				price = (3/4) * movie.getPrice();  
			}
			else if(time.getTimeType().equals("matinee"))
			{
				price = (3/4) * movie.getPrice() * (1/2);
			}
		}
		else if(ageType.equals("senior"))
		{
			if(time.getDay().equalsIgnoreCase("wednesday"))
			{
				price = 0;
			}
			else 
			{
				if(time.getTimeType().equals("primetime"))
				{
					price = (3/4) * movie.getPrice();  
				}
				else if(time.getTimeType().equals("matinee"))
				{
					price = (3/4) * movie.getPrice() * (1/2);
				}
			}
		}
		else
		{
			if(time.getTimeType().equals("primetime"))
			{
				price = movie.getPrice();  
			}
			else if(time.getTimeType().equals("matinee"))
			{
				price = movie.getPrice() * (1/2);
			}
		}
		return price;	
	}

	public String toString()
	{
		return movie.getTitle() + "\n" + this.ageType + "\n" + time.getDay()+ ", " + time.getTime() +
				"\n"  + "Price: $" + getPrice();
			
	}
}