//package astar;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.*;

public class AStar{
	public static final int DIAGONAL_COST = 14;
    public static final int V_H_COST = 10;
    
    static class Cell{  
        int heuristicCost = 0; //Heuristic cost
        int finalCost = 0; //G+H
        int i, j;
        Cell parent; 
        
        Cell(int i, int j){
            this.i = i;
            this.j = j; 
        }
        
        @Override
        public String toString(){
            return "["+this.i+", "+this.j+"]";
        }
    }
    
    static Cell [][] grid = new Cell[32][32];
    
    static PriorityQueue<Cell> open;
    static boolean closed[][];
    static int startI, startJ;
    static int endI, endJ;
    
    public static void setBlocked(int i, int j){
        grid[i][j] = null;
    }
    
    public static void setStartCell(int i, int j){
        startI = i;
        startJ = j;
    }
    
    public static void setEndCell(int i, int j){
        endI = i;
        endJ = j; 
    }
    
    static void checkAndUpdateCost(Cell current, Cell t, int cost){
        if(t == null || closed[t.i][t.j])return;
        int t_final_cost = t.heuristicCost+cost;
        
        boolean inOpen = open.contains(t);
        if(!inOpen || t_final_cost<t.finalCost){
            t.finalCost = t_final_cost;
            t.parent = current;
            if(!inOpen)open.add(t);
        }
    }
    
 public static void AStar(){ 
        
        //add the start location to open list.
        open.add(grid[startI][startJ]);
        
        Cell current;
        
        while(true){ 
            current = open.poll();
            if(current==null)break;
            closed[current.i][current.j]=true; 

            if(current.equals(grid[endI][endJ])){
                return; 
            } 

            Cell t;  
            if(current.i-1>=0){
                t = grid[current.i-1][current.j];
                checkAndUpdateCost(current, t, current.finalCost+V_H_COST); 

                if(current.j-1>=0){                      
                    t = grid[current.i-1][current.j-1];
                    checkAndUpdateCost(current, t, current.finalCost+DIAGONAL_COST); 
                }

                if(current.j+1<grid[0].length){
                    t = grid[current.i-1][current.j+1];
                    checkAndUpdateCost(current, t, current.finalCost+DIAGONAL_COST); 
                }
            } 

            if(current.j-1>=0){
                t = grid[current.i][current.j-1];
                checkAndUpdateCost(current, t, current.finalCost+V_H_COST); 
            }

            if(current.j+1<grid[0].length){
                t = grid[current.i][current.j+1];
                checkAndUpdateCost(current, t, current.finalCost+V_H_COST); 
            }

            if(current.i+1<grid.length){
                t = grid[current.i+1][current.j];
                checkAndUpdateCost(current, t, current.finalCost+V_H_COST); 

                if(current.j-1>=0){
                    t = grid[current.i+1][current.j-1];
                    checkAndUpdateCost(current, t, current.finalCost+DIAGONAL_COST); 
                }
                
                if(current.j+1<grid[0].length){
                   t = grid[current.i+1][current.j+1];
                    checkAndUpdateCost(current, t, current.finalCost+DIAGONAL_COST); 
                }  
            }
        } 
    }
    
 public static void startAStar(int x, int y, int si, int sj, int ei, int ej, int[][] blocked) throws FileNotFoundException{
      //Reset
     grid = new Cell[x][y];
     closed = new boolean[x][y];
     open = new PriorityQueue<>((Object o1, Object o2) -> {
          Cell c1 = (Cell)o1;
          Cell c2 = (Cell)o2;

          return c1.finalCost<c2.finalCost?-1:
                  c1.finalCost>c2.finalCost?1:0;
      });
     //Set start position
     setStartCell(si, sj);  //Setting to 0,0 by default. Will be useful for the UI part
     
     //Set End Location
     setEndCell(ei, ej); 
     
     for(int i=0;i<x;++i){
        for(int j=0;j<y;++j){
            grid[i][j] = new Cell(i, j);
            grid[i][j].heuristicCost = Math.abs(i-endI)+Math.abs(j-endJ);
        }
     }
     grid[si][sj].finalCost = 0;
     
     /*
       Set blocked cells. Simply set the cell values to null
       for blocked cells.
     */
     for(int i = 0; i < x; i++) {
    	 for(int j = 0; j <y; j++) {
    		 if(blocked[i][j] == 1) {
    			 setBlocked(i,j);
    	 		}
    		 }
    	 }
     
     //Display initial map
      for(int i=0;i<x;i++){
          for(int j=0;j<y;j++){
             if(i==si&&j==sj)System.out.print("SO  "); //Source
             else if(i==ei && j==ej)System.out.print("DE  ");  //Destination
             else if(grid[i][j]!=null)System.out.printf("%-3d ", 0);
             else System.out.print("BL  "); 
          }
          System.out.println();
      } 
      System.out.println();
     
     AStar(); 
     File outputmap = new File ("map-out.txt");
     PrintWriter mapout = new PrintWriter(outputmap);
     for(int i=0;i<x;i++){
         for(int j=0;j<y;j++){
             if(grid[i][j]!=null) {
            	 System.out.printf("%-3d ", grid[i][j].finalCost);
            	 mapout.printf("%-3d ", grid[i][j].finalCost);
            	 
             }             
             else {
            	 System.out.print("BL  ");
            	 mapout.print("BL  ");
             }
             if(j == (y-1)) {
        		 System.out.println("");
        		 mapout.println("");
        	 }

         }
         System.out.println();
     }
     mapout.close();
     System.out.println();
      

         //Trace back the path 
          Cell current = grid[endI][endJ];
          Stack<Cell> traceback = new Stack<Cell>();
          Stack<Cell> waypoint = new Stack<Cell>();
          traceback.push(current);
          System.out.print(current);
          while(current.parent!=null){
              System.out.print(" -> "+current.parent);
              traceback.push(current.parent);
              waypoint.push(current.parent);
              current = current.parent;
          } 
          System.out.println();
         // System.out.print("[-6, -6] -> [" + (traceback.peek().i-16)/(-2.0) + ", " + (traceback.peek().j-16)/2.0 + "]");
          //traceback.pop();
          while(!traceback.isEmpty()) {
        	  System.out.print("[" + (traceback.peek().i-15)/(-2.0) + ", " + (traceback.peek().j-16)/2.0 + "] ->");
        	  traceback.pop();
          }
          //System.out.print(" [6.5, 6.5]");
          System.out.println();
          waypointGen(waypoint);

    
}

public static void waypointGen(Stack<Cell> waypoint) throws FileNotFoundException {
	File planout = new File("plan-out.txt");
	PrintWriter plan = new PrintWriter(planout);
	int counter = 0;
	Queue<Integer> waypointList = new LinkedList<Integer>(); 
	int dirI = 1;
	int dirJ = -1;
	int prevI = 27;
	int prevJ = 4;
	waypoint.pop();
	while(!waypoint.isEmpty()) {
		if((prevI-waypoint.peek().i == dirI) && (prevJ-waypoint.peek().j == dirJ)) {
			prevI = waypoint.peek().i;
			prevJ = waypoint.peek().j;
			System.out.println("Same Direction");
		}
		else {
			dirI = prevI-waypoint.peek().i;
			dirJ = prevJ-waypoint.peek().j;
			prevI = waypoint.peek().i;
			prevJ = waypoint.peek().j;
			waypointList.add(prevI);
			waypointList.add(prevJ);
			System.out.println("Direction Changed, I: " + prevI + " J: " + prevJ);
			counter += 2;
		}
		waypoint.pop();				
	}
	System.out.print(counter);
	plan.print(counter);
	while(!waypointList.isEmpty()) {
		System.out.print(" " + (waypointList.peek()-15)/(-2.0) + " " + (waypointList.peek()-16)/(2.0));
		plan.print(" " + (waypointList.peek()-15)/(-2.0) + " " + (waypointList.peek()-16)/(2.0));
		waypointList.poll();
	}
	plan.close();
	
	
	
}

public static void main(String[] args) throws Exception{
	int map[][] = new int[32][32];
	File mapFile = new File("map.txt");
	Scanner mapReader = new Scanner(mapFile);
	for(int i = 0; i < 32; i++) {
		for(int j = 0; j < 32; j++) {
				map[i][j] = mapReader.nextInt();			
		}
	}
	mapReader.close();
  startAStar(32, 32, 27, 4, 3, 28, map); 

}
}




