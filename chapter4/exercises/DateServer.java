package com.app;

import java.net.*;
import java.io.*;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


public class DateServer
{
    public static class Request implements Callable<Integer>{
        private Socket client;

        public Request(Socket s){
            this.client = s;
        }

        @Override
        public Integer call() throws Exception {
            PrintWriter pout = null;
            try {
                pout = new PrintWriter(client.getOutputStream(), true);
            } catch (IOException e) {
                e.printStackTrace();
            }
            pout.println();
            try {
                client.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return 0;
        }
    }

    public static void main(String[] args) throws IOException {
        ServerSocket sock = new ServerSocket (6017);
        Socket client = sock.accept();

            /* now listen for connections */
            while (true) {
                ExecutorService pool = Executors.newSingleThreadExecutor();
                pool.submit(new Request(client));
            }
    }
}