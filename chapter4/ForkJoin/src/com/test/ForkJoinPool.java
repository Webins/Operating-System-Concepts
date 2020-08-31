package com.test;

public class ForkJoinPool extends java.util.concurrent.ForkJoinPool {

    public static void main(String[] args) {
        ForkJoinPool pool = new ForkJoinPool();

        int array[] = new int[1500];
        for(int i = 0, j = 0; i < array.length; i++){
            array[i] = j;
            j+= i;
        }


        System.out.println("Threads parallel");
        long start = System.nanoTime();
        SumTask sumTask = new SumTask(0, array.length - 1, array);

        int sum = pool.invoke(sumTask);

        long finish = System.nanoTime() - start;

        System.out.println("Result is = " + sum);
        System.out.println("Computed in : " + finish + " Nano seconds");

        System.out.println("Normal sum");

        start = System.nanoTime();

        int sum2 = 0;
        for(int i = 0; i < array.length; i++){
            sum2 += array[i];
        }

        finish = System.nanoTime() - start;

        System.out.println("Result is = " + sum2);
        System.out.println("Computed in : " + finish + " Nano seconds");




    }
}
