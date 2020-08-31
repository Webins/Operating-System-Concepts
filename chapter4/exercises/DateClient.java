package com.app;
import java.net.*;
import java.io.*;
public class DateClient
{
    public static void main(String[] args) throws IOException {
            /* make connection to server socket */
            Socket sock = new Socket ("127.0.0.1",6013);
            InputStream in = sock.getInputStream();
            BufferedReader bin = new
                    BufferedReader(new InputStreamReader(in));
            /* read the date from the socket */
            String line;
            while ( (line = bin.readLine()) != null)
                System.out.println(line);
            /* close the socket connection*/
            sock.close();

    }
}

