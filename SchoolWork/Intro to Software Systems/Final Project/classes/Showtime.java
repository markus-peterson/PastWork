package classes;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class Showtime{
	
	private int hour;
	private String day, minute;
	
	public Showtime(int hour, String day) {
		this.hour = hour;
		this.minute = "00";
		this.day = day;
	}

	public String getTime() { return hour + ":" + minute; }

	public int getHour() { return hour; }

	public void setHour(int hour) {
		this.hour = hour;
	}

	public String getMinute() {
		return minute;
	}
	
	public String getDay() {
		return day;
	}

	public void setDay(String day) {
		this.day = day;
	}

	public String getTimeType() {
		DateTimeFormatter date = DateTimeFormatter.ofPattern("HH");
		LocalDateTime now = LocalDateTime.now();
		
		int hourShown = Integer.parseInt(date.format(now));
		String timeType = "";
		
		if(hourShown >= 19 && hourShown <= 22) {
			timeType = "primetime";
		}
		else {
			timeType = "matinee";
		}
		return timeType;
	}
}