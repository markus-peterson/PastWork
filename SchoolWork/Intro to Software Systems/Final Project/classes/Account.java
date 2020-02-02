package classes;

import java.util.ArrayList;

public class Account {

	private String userName, password;
	private ArrayList<Payment> payments;
	
	public Account(String userName, String password) {
		this.userName = userName;
		this.password = password;
	}

	public String getUserName() {
		return userName;
	}

	public void setUserName(String userName) {
		this.userName = userName;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}
	
	public ArrayList<Payment> getPayments() {
		return this.payments;
	}
	public void addPayment(Payment p) {
		this.payments.add(p);
	}
	public String toString() {
		return this.userName + " " + this.password;
	}
}
