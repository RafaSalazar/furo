package furoWifi;

import java.io.IOException;
import java.net.URL;
import java.util.Scanner;

public class prueba {

	public static void main(String[] args) throws IOException, InterruptedException  {

		String link ="http://192.168.137.107:80";
		FuroConection furo1 = new FuroConection(link);
		
		furo1.send("dd");
        boolean B = true;
       
		Scanner s = new Scanner(System.in);
		String in;
		
//		for(int i = 0; i<10;i++){
//			furo1.send("w");
//			Thread.sleep(500);
//			furo1.send("p");
//			Thread.sleep(500);
//			furo1.send("d");
//			Thread.sleep(500);
//			furo1.send("p");
//			Thread.sleep(500);
//		}
		
		
		
		while(true){
			in = s.nextLine();
			furo1.send(in);
			System.out.println(in);
		}
		
	}
	
}
