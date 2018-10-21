
import java.util.Arrays;

public class PuzzleGame {

    public static void main(String[] args) {
        Node node1 = new Node();
        Node node2 = new Node();
        int status1[] = {0,2,1,6,7,5,3,8,4};
        int status2[] = {0,1,2,3,4,5,6,7,8};
        node1.setStatus(status1);
        node2.setStatus(status2);
        Fringe fringe = new Fringe();
        ListStatus ls = new ListStatus();
        
        AStar aStar = new AStar();
        Node kq = aStar.runAStar(node1, node2, fringe, ls);
        aStar.result(kq);
        System.out.println("so nut tao ra: " + aStar.soNutTaoRa);
        
    }
    
}

class Node {
    private int[] status = new int[9];
    private int f; // f(n) = g(n) + h(n)
    private int g;
    private int h;
    private Node parent;
    public Node() {
        f = 0;
        g = 0;
        h = 0;
        parent = null;
        Arrays.fill(status, 0);
    }
    public int getF() {
        return f;
    }
    public void setF(int f) {
        this.f = f;
    }
    public int getG() {
        return g;
    }
    public void setG(int g) {
        this.g = g;
    }
    public int getH() {
        return h;
    }
    public void setH(int h) {
        this.h = h;
    }
    public void setParent(Node parent) {
        this.parent = parent;
    }
    public Node getParent() {
        return parent;
    }
    public void setStatus(int status[]) {
        int i;
        for(i = 0; i < 9; i++) {
            this.status[i] = status[i];
        }
    }
    public int[] getStatus() {
        return status;
    }
    public void printStatus() {
        for(int i = 0; i < 9; i++) {
            if(status[i] != 0) {
                System.out.printf("%d ", status[i]);
                if((i%3) == 2)  System.out.println();
            }
            else {
                System.out.printf("  ", status[i]);
                if((i%3) == 2)  System.out.println();
            }
        }
        System.out.println("--------------\n");
    }
    public int indexBlank() {
        int i;
        for(i = 0; i < 9; i++) {
           if(status[i] == 0)    break;
        }
        return i;
    }
    // di chuyen o trong len tren
    public boolean moveUp() {
        // Tim kiem o trong
        int i = indexBlank();
        if((i/3) != 0) {
            status[i] = status[i-3];
            status[i-3] = 0;
            return true;
        }
        return false;
    }
    public boolean moveDown() {
        // Tim kiem o trong
        int i = indexBlank();
        if((i/3) != 2) {
            status[i] = status[i+3];
            status[i+3] = 0;
            return true;
        }
        return false;
    }
    public boolean moveLeft() {
        // Tim kiem o trong
        int i = indexBlank();
        if((i%3) != 0) {
            status[i] = status[i-1];
            status[i-1] = 0;
            return true;
        }
        return false;
    }
    public boolean moveRight() {
        // Tim kiem o trong
        int i = indexBlank();
        if((i%3) != 2) {
            status[i] = status[i+1];
            status[i+1] = 0;
            return true;
        }
        return false;
    }
    
    public int heuristic1(int[] statusEnd) {
        int i, j, heuristic1 = 0;
        for(i = 0; i < 9; i++) {
            for(j = 0; j < 9; j++) {
                if(status[i] == statusEnd[j]) { // tim vi tri status[i] o StatusEnd
                    heuristic1 += Math.abs((i/3)-(j/3)) + Math.abs((i%3)-(j%3));
                }
            }
        }
        return heuristic1;
    }
    
    
}

class Fringe {
    public static Node[] number = new Node[362880];    // luu cac trag thai
    public static int count;   // dem so luong cac trang thai
    
    // khoi tao hang doi rong
    public Fringe() {
       count = 0;
    }
    
    // Kiem tra hang doi rong
    public boolean isEmpty() {
        if(count == 0)  return true;
        return false;
    }
    
    // Them mot trang thai vao fringe
    public void push(Node N) {
        number[count] = N;
        count++;
    }
    
    // Xoa mot trang thai
    public Node pop() {
        if(isEmpty()) {
            System.out.println("Fringe rong!");
            return null;
        }
        else {
            int i, j=0;
            // Tim fringe min
            Node fMin = number[0];
            for(i = 0; i < count; i++) {
                if(number[i].getF() < fMin.getF()) {
                    fMin = number[i];
                    j = i;  // luu lai vi tri status co f min
                }
            }
            for(i = j; i < count-1; i++) {
                number[i] = number[i+1];
            }
            count--;
            return fMin;
        }
    }
}

// M: tap cac trang thai tiep theo
class ListStatus {
    public static Node[] number = new Node[4];
    public static int count;
    
    // Khoi tao 
    public ListStatus() {
        count = 0;
    }
    
    // danh sach cac trang thai tiep theo cua n(i)
    public void nextStatus(Node N) {
        if(N.moveUp()) {    // neu o trong di chuyen len dc thi...
            number[count] = new Node();
            number[count].setStatus(N.getStatus()); // sao chep trang thai hien tai cua nut cha
            number[count].setParent(N);
            count++;
            N.moveDown();   // di chuyen o trong ve cho cu
        }
        if(N.moveDown()) {    // neu o trong di chuyen len dc thi...
            number[count] = new Node();
            number[count].setStatus(N.getStatus()); // sao chep trang thai hien tai cua nut cha
            number[count].setParent(N);
            count++;          
            N.moveUp();   // di chuyen o trong ve cho cu
        }
        if(N.moveLeft()) {    // neu o trong di chuyen len dc thi...
            number[count] = new Node();
            number[count].setStatus(N.getStatus()); // sao chep trang thai hien tai cua nut cha
            number[count].setParent(N);
            count++;
            N.moveRight();   // di chuyen o trong ve cho cu
        }
        if(N.moveRight()) {    // neu o trong di chuyen len dc thi...
            number[count] = new Node();
            number[count].setStatus(N.getStatus()); // sao chep trang thai hien tai cua nut cha
            number[count].setParent(N);
            count++;
            N.moveLeft();   // di chuyen o trong ve cho cu
        }
    }
}

class AStar {
    public Node runAStar(Node begin, Node end, Fringe F, ListStatus LS) {
        int i, j, result;
        // nap begin vao fringe
        F.push(begin);
        Node p; // nut cha
        while(F.isEmpty() == false) {
            p = F.pop();
            
            if(Arrays.equals(p.getStatus(), end.getStatus())) {
                return p;
            }
            else {
                
                LS.nextStatus(p);
                for(i = 0; i < ListStatus.count; i++) {
                    // tinh f = g + h;
                    ListStatus.number[i].setG(p.getG()+1);
                    ListStatus.number[i].setH(ListStatus.number[i].heuristic1(end.getStatus()));
                    ListStatus.number[i].setF(ListStatus.number[i].getH() + ListStatus.number[i].getG());
                    ListStatus.number[i].setParent(p);
                    
                    // kiem tra vao push cac trang thai vao fringe
                    if(p.getParent() != null) {
                        // neu trang thai dang xet khong trung vs cha cua trang thai p
                        if(!Arrays.equals(ListStatus.number[i].getStatus(), p.getParent().getStatus())) {
                            result = 0;
                    //System.out.println(ListStatus.number[i].getF());
                    //ListStatus.number[i].printStatus();
                            for(j = 0; j < Fringe.count; j++) {
                                if(Arrays.equals(ListStatus.number[i].getStatus(), Fringe.number[j].getStatus())) {
                                    result = 1;
                                    break;
                                }
                            }
                            if(result == 0) F.push(ListStatus.number[i]);
                        }
                        
                    }
                    else {                       
                        F.push(ListStatus.number[i]);
                    }
                    soNutTaoRa++;
                }
                ListStatus.count = 0;
            }
        }
        return null;
    }
    int k = 0;
    int soNutTaoRa = 0;
   public void result(Node end) {
       
       Node p = end;
       if(p != null) {
           result(p.getParent());
           System.out.printf("%d, f = %d, g = %d, h = %d\n", k, p.getF(), p.getG(), p.getH());
           p.printStatus();
           k++;
       }
   }
}

