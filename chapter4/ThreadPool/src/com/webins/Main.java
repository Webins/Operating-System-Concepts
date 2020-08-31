package com.webins;


import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;


public class Main {

    public static void main(String[] args) {
        AtomicInteger count = new AtomicInteger();
        Runnable Task = () -> System.out.println("Task Number: "+ (count.getAndIncrement()));
        Integer numTasks = 5;

        /**Create the thread pool**/
        ExecutorService pool = Executors.newCachedThreadPool();

        /**Run each task using a thread in the pool**/
        for(int i = 0; i < numTasks; i++)
            pool.execute(Task);

        /**Shut down the pool once all threads have completed**/
        pool.shutdown();
    }
}
