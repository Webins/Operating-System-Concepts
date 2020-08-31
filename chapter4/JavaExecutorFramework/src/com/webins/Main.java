package com.webins;

import java.util.concurrent.*;

class Summation implements Callable<Integer>{ /*Callable can return a result, a thread can't*/

    private int upper;

    public Summation(int upper) {
        this.upper = upper;
    }

    /**The thread will execute in this method**/
    @Override
    public Integer call() throws Exception {
        int sum = 0 ;
        for(int i = 1; i <= upper; i++){
            sum+=i;
        }
        return sum;
    }
}

public class Main {

    public static void main(String[] args) throws ExecutionException, InterruptedException {
        int upper = Integer.parseInt(args[0]);

        ExecutorService pool = Executors.newSingleThreadExecutor(); /*this doesnt return a result*/
        Future<Integer> result = pool.submit(new Summation(upper)); /*it return a result that is called a Future*/

        /*Once we submit the callable task to the thread, we wait for its
        result by calling the get() method of the Future object it returns.*/
        System.out.println("Sum = " + result.get());
    }
}
