package com.webins;

public class Main {

    public static void main(String[] args) throws InterruptedException {
//        Runnable task = new Runnable() {
//            @Override
//            public void run() {
//                System.out.println("I'm a thread");
//            }
//        };

        /**Is the same thing as above*/
	    Runnable task = () ->{
            System.out.println("I'm a thread");
        };

	    Thread worker = new Thread(task);
	    worker.start();
	    /**
         * When we invoke the start method, we initializes a new thread in the JVM
         * and we call the run method making the thread eligible to be run by the JVM
         * the run method is called implicitly when we invoke the start method
         * **/

        /** As wait in phtread and WaitForSingleObject in windows, the join method waits the thread to finish before proceeding**/
	    worker.join();



    }
}
