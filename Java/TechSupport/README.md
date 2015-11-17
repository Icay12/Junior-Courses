Java Lab2: TechSupport
---------------------------
#requirement

TechSupport is a program intended to provide technical support for customers of the fictitious 

DodgySoft company.  Some time ago, DodgySoft had a technical support department with people sitting at telephone where customers could call to get advice and help with their technical problems with the DodgySoft products.  Recently, though, business has not been going so well, and DodgySoft decided to get rid of the technical support department to save money.  They now want to develop the  TechSupport system to give the impression that support is still provided.  The system is supposed to mimic the responses a technical support person might give.  Customers can communicate with the technical support system online.

The plan is that we shall have a set of words that are likely to occur in typical questions and we will associate these words with particular responses.  If the input from the user contains one of our 
known words we can generate a related response.  To make it more nature, one word may be associated with several different answers which will be randomly picked when the word encountered.

The user interface is quite straightforward: The user inputs a sentence and the program answers.  Your program should be able to split the user input into words and find "key words" inside them. Then for every key word, try to decide a "suitable" (random) answer and print it out.  For a sentence with more than one key word, you may have your own way to deal with it, like to print only one or all of them.

#notice

������װ���˼����򵥵Ĺؼ��� apple, lemon, grape, banana ���ʿ���Ӧ������

������������������ʵľ��Ӻ󣨴�֮���Կո�ֿ��������Զ����Ԥ��Ļظ�

һ���ؼ��ʻظ�һ��response�������ж���ؼ���ʱ���ظ�ÿ���ؼ��ʵ�һ��Ԥ��response