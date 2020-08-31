package com.test;

import java.util.concurrent.RecursiveTask;

public class SumTask extends RecursiveTask<Integer> {
   static final int THRESHOLD = 1000;
   private int begin;
   private int end;
   private int array[];

    public SumTask(int begin, int end, int[] array) {
        this.begin = begin;
        this.end = end;
        this.array = array;
    }

    @Override
    protected Integer compute() {
        if(end - begin < THRESHOLD){
            int sum = 0;
            for(int i = 0; i < array.length; i++)
                sum += array[i];

            return sum;
        }else{
            int mid = (begin + end) / 2;

            SumTask leftTask = new SumTask(begin, mid, array);
            SumTask rightTask = new SumTask(mid, end, array);

            /*Run in parallel*/
            leftTask.fork();
            rightTask.fork();

            return leftTask.join() + rightTask.join();

        }
    }
}
