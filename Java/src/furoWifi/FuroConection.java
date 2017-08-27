package furoWifi;

import java.io.IOException;
import java.io.OutputStreamWriter;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.net.URLConnection;

public class FuroConection {

	private URL url;
    private URLConnection conn;
    private OutputStreamWriter wr;
    
    public FuroConection(String link)throws IOException, InterruptedException{
    	
    	url = new URL(link);
    	
    }
        
    public void send(String s) throws IOException, InterruptedException{
    	try{
    		conn = url.openConnection();
    		conn.setReadTimeout(1);
            conn.setDoOutput(true);
    		wr = new OutputStreamWriter(conn.getOutputStream());
    		wr.write("?"+s+" ?@?");
    		wr.close();
    		conn.getInputStream();
    		}catch(SocketTimeoutException e ){
    			Thread.sleep(50);
    		}
    	
    }
}
