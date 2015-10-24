package rpnCal;

import java.util.Scanner;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Stack;
import java.lang.Character;

public class RPNCal {
    //symbol contains the basic operators
    static ArrayList<String> symbol = new ArrayList(Arrays.asList(new String[]{"+","-","*","/","(",")"}));

    public ArrayList<String> expressionSplit(String expression) 
    {
        ArrayList<String> result = new ArrayList<String>();

        char[] singleCharacter = expression.toCharArray();

        String[] singleString = new String[singleCharacter.length];
        for(int i = 0; i < singleCharacter.length; ++i)
        {
            singleString[i] = String.valueOf(singleCharacter[i]);
        }

        String value = "";
        for(String i : singleString)
        {
            if(symbol.contains(i))
            {
                if(value != "")
                {
                    result.add(value);
                }
                result.add(i);
                value = "";
            }
            else
            {
                value += i;
            }
        }
        if(value != "")
        {
            result.add(value);
        }
    	
        return result;
    }

    public boolean lowerPriority(String a, String b)
    {
    	if(b.equals("("))
    	{
    		return false;
    	}
    	if(a.equals("+") || a.equals("-"))
    	{
    		return true;
    	}
    	else
    	{
    		return false;
    	}
    }

    public ArrayList<String> preToPos(ArrayList<String> pre) throws java.util.EmptyStackException
    {
        ArrayList<String> pos = new ArrayList(pre.size());
        Stack<String> stackForConvert = new Stack<String>();
        //if any wrong format, throw exception
        try{
        	
            for(String a : pre)
            {
            	if(!symbol.contains(a))
            	{
            		pos.add(a);
            		continue;
            	}
            	if(a.equals("(") || stackForConvert.isEmpty())
            	{
            		stackForConvert.push(a);
            		continue;
            	}
            	if(a.equals(")"))
            	{
            		while(!stackForConvert.peek().equals("("))
            		{
            			pos.add(stackForConvert.pop());
            		}
            		stackForConvert.pop();
            		continue;
            	}
            	
            	while(lowerPriority(a,stackForConvert.peek()))
            	{
            		pos.add(stackForConvert.pop());
            		if(stackForConvert.isEmpty())
            			break;
            	}
            	stackForConvert.push(a);
            }
            while(!stackForConvert.isEmpty())
            {
            	pos.add(stackForConvert.pop());
            }

        }catch(java.util.EmptyStackException e)
        {
        	System.out.println("Wrong format!");
            System.exit(1);
        }
        return pos;
    }

    public int Calculate(ArrayList<String> pos) throws java.util.EmptyStackException,java.lang.ArithmeticException,java.lang.NumberFormatException
    {
    	Stack<String> stackForCal = new Stack<String>();
    	int result = 0;
    	try{
    	for(String a : pos)
    	{
    		if(!symbol.contains(a))
    		{
    			stackForCal.push(a);
    			continue;
    		}
        	int temp = 0;
    		int op1 = Integer.valueOf(stackForCal.pop());
    		int op2 = Integer.valueOf(stackForCal.pop());
    		switch(a)
    		{
    		case "+": temp = op2 + op1; break;
    		case "-": temp = op2 - op1; break;
    		case "*": temp = op2 * op1; break;
    		case "/": temp = op2 / op1; break;
    		}
    		stackForCal.push(String.valueOf(temp));

    	}
    	result = Integer.valueOf(stackForCal.pop());
    	}catch(java.util.EmptyStackException e)
    	{
    		System.out.println("Wrong format!");
    		System.exit(1);
    	}catch(java.lang.NumberFormatException e)
    	{
    		System.out.println("Wrong character!");
    		System.exit(1);
    	}catch(java.lang.ArithmeticException e)
    	{
    		System.out.println("Wrong math!");
    		System.exit(1);    		
    	}
    	return result;
    }

    public static void main(String[] args)
    {

        Scanner in = new Scanner(System.in);
        String expression = in.nextLine();
        RPNCal test = new RPNCal();

        //print pre expression
        ArrayList<String> a = new ArrayList(test.expressionSplit(expression));
        System.out.println(a);

        //print pos expression
        a = test.preToPos(a);
        System.out.println(a);
        
        //print result
        int result = test.Calculate(a);
        System.out.println(result);      
    }

}
