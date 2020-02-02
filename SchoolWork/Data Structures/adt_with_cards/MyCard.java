package adt_with_cards;

public class MyCard implements Card{
	
	private String[] deck = new String[52];
	private String[] suit = new String[] {"S", "C", "H", "D"};
	
	@Override
	public void initialize() {
		int count = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 1; j < 14; j++) {
				if (j == 1) {
					deck[count] = "Ace" + suit[i];
				} else if (j == 11) {
					deck[count] = "Jack" + suit[i];
				} else if (j == 12) {
					deck[count] = "Queen" + suit[i];
				} else if (j == 13) {
					deck[count] = "King" + suit[i];
				} else {
					deck[count] = j + suit[i];
				}
				System.out.println(deck[count]);
				count++;
			}
		}
	}

	@Override
	public String drawCard() {
		int rand = (int)((Math.random() * 52));
		
		return deck[rand];
	}
	
	public static void main(String[] args) {
		MyCard mc = new MyCard();
		mc.initialize();
		System.out.println(mc.drawCard());
	}
}
