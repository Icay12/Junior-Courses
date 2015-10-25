package TechSupport;

import java.util.Scanner;
import java.util.ArrayList;
import java.util.Random;
import java.lang.String;

class KeyWord{
	private String m_word;
	private Random random = new Random();
	private ArrayList<String> m_response = new ArrayList<String>();
	public KeyWord(String word){
		m_word = new String(word);
	}
	public String GetKeyWord(){
		return m_word;
	}
	public void addResponse(String response){
		m_response.add(response);
	}
	public String getResponse(){
		int size = m_response.size();
		if(size == 0){
			return "Hello,I don't know what it is...";
		}
		else{
			return m_response.get(random.nextInt(size));
		}
	} 
	
}

class Library{
	private ArrayList<KeyWord> m_theKeys = new ArrayList<KeyWord>();
	public String getResponse(String word){
		for(KeyWord item : m_theKeys){
			if(item.GetKeyWord().equals(word)){
				return item.getResponse();
			}
		}
		return "";
	}
	public Library(){
		KeyWord apple = new KeyWord("apple");
		apple.addResponse("Apples are red.");
		apple.addResponse("I like eating apples~");
		apple.addResponse("An apple a day, keeps doctors away!");
		
		KeyWord lemon = new KeyWord("lemon");
		lemon.addResponse("Lemons are yellow.");
		lemon.addResponse("And all that I can see is just another lemon tree~~");
		
		KeyWord grape = new KeyWord("grape");
		grape.addResponse("Grapes are purple.");
		
		KeyWord banana = new KeyWord("banana");
		
		m_theKeys.add(apple);
		m_theKeys.add(lemon);
		m_theKeys.add(grape);
		m_theKeys.add(banana);
	}
	
}
public class TechSupport {
	private static Library m_library = new Library();
	public static ArrayList<String> questionSplit(String question){
		question = question.toLowerCase();
		question = question.replaceAll("[\\pP¡®¡¯¡°¡±]", "");
		String questionSplitTemp[] = question.split(" ");
		ArrayList<String> questionSplited = new ArrayList<String>();
		for(String item : questionSplitTemp){
			questionSplited.add(item);
		}
		return questionSplited;	
	}
	public static void autoResponse(ArrayList<String> keywords){
		for(String item : keywords){
			String res = new String();
			res = m_library.getResponse(item);
			if(res == ""){
				continue;
			}
			System.out.println(res);
		}
	}
	public static void main(String[] args) {
		Scanner in = new Scanner(System.in);
		
		while(true){
			System.out.println("input your question or <exit>.");
			String input = in.nextLine();
			input = input.toLowerCase();
			if(input.equals("exit")){
				break;
			}
			ArrayList<String> words = TechSupport.questionSplit(input);
			TechSupport.autoResponse(words);
		}
		

	}

}
