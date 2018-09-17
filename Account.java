public class Account {
    private double balance;
    private Account parentAccount;
    public Account(double initialbalance,Account parentAccount){
        this.balance=initialbalance;
        this.parentAccount=parent Account;}
    public Account(double initialbalance){
        this.balance=initialbalance;
        this.parentAccount=null;
        }
    public void withdraw(double n){
        if (this.parentAccount=null){
            if (this.balance-n<0) {return false;}
             else {this.balance=this.balance-n;return true;}}

        else{ if (this.balance-n<0) {this.balance=0;
                this.parentAccount.balance=this.parentAccount.balance+this.balance-n}
            else {this.balance=this.balance-n;}
            return true;   }
        }

    public void getBalance(){
        return this.balance}
    public void merge(Account n){
        double newmoney=n.balance
        this.balance=this.balance+newmoney
        }
}



//public class TestAccount{
//    public static void main(String[] args){
//        Account





//    }

//public class Counter {

//    int count = 0;

//    void increment() {
//        count = count + 1;
//    }

//    public static void main(String[] args) {
//       Counter c1 = new Counter();
//        c1.increment();
//        Counter c2 = new Counter();
//        c1 = c2;
//    }
//}
