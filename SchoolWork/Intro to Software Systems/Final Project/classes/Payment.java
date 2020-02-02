package classes;

import java.util.ArrayList;

public class Payment 
{
	
	private ArrayList<Ticket> ticketList;
	private String payMethod;
	
	public String getPayMethod() {
		return payMethod;
	}

	public void setPayMethod(String payMethod) {
		this.payMethod = payMethod;
	}

	
	public Payment(ArrayList<Ticket> ticketList, String payMethod) 
	{
		this.payMethod = payMethod;
		this.ticketList = ticketList;
	}
	
	public void addTicket(Ticket t) {
		if (t.isPurchasable()) {
			ticketList.add(t);
		}
	}
	
	public double getTotal() 
	{
		double total = 0;
		double childrenCountSat = 0;
		double adultCount = 0;
		
		for(int i = 0; i<ticketList.size();i++)
		{
			if(ticketList.get(i).getAgeType().equals("adult"))
			{
				adultCount++;
			}
			if(ticketList.get(i).getTime().getDay().equalsIgnoreCase("saturday")&&ticketList.get(i).getTime().getTimeType().equals("matinee"))
			{
				if(ticketList.get(i).getAgeType().equals("child"))
				{
					childrenCountSat++;
				}
			}
			if(childrenCountSat>0 && ticketList.get(i).getAgeType().equals("child") && ticketList.get(i).getTime().getTimeType().equals("matinee"))
			{
				ticketList.get(i).setPrice(0);
				childrenCountSat--;
			}
			if(adultCount >= 2)
			{
				for(int j = 0; j<ticketList.size();j++)
				{
					if(ticketList.get(j).getAgeType().equalsIgnoreCase("child"))
					{
						ticketList.get(j).setPrice(ticketList.get(j).getPrice()*0.5);
					}
				}
			}
		}
		
		for(Ticket prices : ticketList)
		{
			total += prices.getPrice();
		}
		return total;
	}
	
	public String toString()
	{
		String printed = "";

		for(Ticket ticket : ticketList)
		{
			printed += ticket.toString() + "\n" + "\n";
		}
		printed += "Total Payment: " + this.getTotal();

		return printed;
	}
	
}