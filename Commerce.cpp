/*Author      : Chandrakumar
*/
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
//0-just created 1-yet to be delivered 2- delivered 3-cancled and not taken 4-rovoked from customer

#define CREATED 0
#define NOTDELIVERED 1
#define DELIVERED 2
#define CANCLEDNOTTAKEN 3
#define REVOKED 4

#define ERROR -1

using namespace std;

struct Timestamp{
  int day,month,year,hour,min;
};

typedef struct Timestamp Time;
typedef float Money;
typedef int ID;

Time getCurrentTime(){
  Time time;
  time.day=10;
  time.month=10;
  time.year=2017;
  time.hour=14;
  time.min=59;
  return time;
}//TODO Change the logic

ID pId=0;
class Product {
private:
  char item[20];
  int quantity;
  Money money;
  ID id;
public:
  Product(){
    quantity =0;
    id = pId++;
  }
  Product(Product *p){
    strcpy(item,p->item);
    quantity = p->quantity;
    money = p->money;
    id = p->id;
  }
  char* getItem(){
    return item;
  }
  int getQuantity(){
    return quantity;
  }
  Money getMoney(){
    return money;
  }
  int getId(){
    return id;
  }
  int updateItem(int qty,Money amt,char *name=NULL,int i=0){
    if(qty>0){
      strcpy(item,name);
      money = amt;
      quantity = qty;
      id = i;
      return quantity;
    }
    else if(qty<0){
      if(quantity+qty>=0){
        quantity+=qty;
        money=qty*amt;
        return quantity;
      }
      else{
        return -2;//denotes nonsufficient
      }
    }
    else return ERROR;//something went wrong
  }
  friend std::ostream  &operator<<( std::ostream &output,const Product &D ) {
    std::cout << D.item<<'\t'<<D.quantity<<'\t'<<D.money/D.quantity<<'\t'<<D.money << '\n';
    return output;
  }
};


ID cartId = 0;
class Cart {
private:
  Product *product;
  Time created_time,delivery_time;
  ID id;
  char address[50];
  ID customerId;
  int status;//0-just created 1-yet to be delivered 2- delivered 3-cancled and not taken 4-rovoked from customer
public:
  void newCart (Time *t,Product *p,char *add,ID i){
    created_time = *t;
    product = p;
    status = CREATED;
    id = cartId++;
    strcpy(address,add);
    customerId = i;
  }
  void ship(char *add){
    strcpy(address,add);
    status = NOTDELIVERED;
  }
  int getStatus(){
    return status;
  }
  bool isCancable(){
    return true;
  }//TODO Change logic
  friend std::ostream  &operator<<( std::ostream &output,const Cart &D ){
    std::cout << '\n';
    std::cout << "NAME\tqty\tprice\ttotal" << '\n';
    for(size_t i=0;i<sizeof(product)/sizeof(product);i++){
      std::cout << D.product[i] << '\n';
    }
    return output;
  }
};


ID cId=0;
class Customer {
private:
  ID id;
  char name[20];
  char email_id[25];
  char address[100][5];
  int addressUsed;
  Cart cart[1000];
  int cartUsed;
  bool log_in;
protected:
  void removeShipedCarts(){
    //cartUsed = 0;
    for(int i=0;i<cartUsed;i++){
      if(cart[i].getStatus()==2){
        if(!cart[i].isCancable()){
          for(int j=i;j<cartUsed;j++){
            Cart *temp=&cart[i];
            cart[j]=cart[i];
            cart[j]=*temp;
          }
        }
      }
    }
  }
public:
  friend std::ostream  &operator<<( std::ostream &output,const Customer &D ){
    std::cout << D.name<<' '<<D.addressUsed;
    return output;
  }
  bool logIn(char *na,char *email){
    log_in = (strcmp(na,name)&&strcmp(email,email_id))?true:false;
    return log_in;
  }
  Customer (char *ad,char *n,char *e){
    strcpy(address[0],ad);
    strcpy(name,n);
    strcpy(email_id,e);
    addressUsed++;
    //cart = (Cart*)malloc(1000*sizeof(Cart));
    cartUsed=0;
    id=cId++;
    log_in=false;
  }
  void addToCart(Product *product,int addSta){
    if(log_in){
      if(cartUsed>1000)
      removeShipedCarts();
      cart[cartUsed++].newCart(new Time(getCurrentTime()),product,address[addSta],id);
      addressUsed = addSta;
    }
  }
  void addAddress(char *add){
    if(log_in)
    strcpy(address[addressUsed++],add);
  }
  void shipCartToAddress(int cartId,int addId){
    if(log_in){
      if(cartId<=cartUsed){
        cart[cartId].ship(address[addId]);
      }
    }
  }
  void logOut(){
    log_in = false;
  }
  char* getName(){
    return name;
  }
  char* getEmail(){
    return email_id;
  }
  char* getAddress(int id){
    return address[id];
  }
  Cart getCart(int id){
    return cart[id];
  }
};

ID wId=0;
class WearHouse {
private:
  char address[30];
  Product product[1000];
  Time time[1000];//Can have upto 1000 products
  ID id;
  int productUsed=0;
public:
  WearHouse (char *add){
    strcpy(address,add);
    id=wId++;
  }
  int addProduct(Product *pro){
    if(productUsed>1000){
      return ERROR;

    }
    else{
      product[productUsed]=pro;
      time[productUsed] = getCurrentTime();
      return product[productUsed++].getQuantity();
    }
  }
  bool removeProduct(Product *pro){
    for (int i = 0; i < productUsed; i++) {
      if(pro->getId()==product[i].getId()){
        for (int j = i; j < productUsed-1; j++) {
          Product *temp = &product[j];
          Time *timeTemp = &time[j];
          time[j]=time[j+1];
          time[j+1]=*timeTemp;
          product[j]=product[j+1];
          product[j+1]=*temp;
        }
        productUsed--;
        return true;
      }
    }
    return false;
  }
};

#ifndef max_delivery
#define max_delivery 200
#endif

ID deliveryBoyId=0;
class DeliveryBoy {
private:
  ID id;
  char name[30];
  Money earned;
  int successfull_deliv_made,deliv_yet_to_be_complete;
  int customer_id[max_delivery];

  Money getEarning(){
    return 20.20;
  }
protected:
  bool cleanUpSize(){
    if(deliv_yet_to_be_complete !=0 )
    return false;
    successfull_deliv_made = 0;
    deliv_yet_to_be_complete = 0;
    return true;
  }
public:
  DeliveryBoy(char *n){
    strcpy(name,n);
    id=deliveryBoyId++;
    earned = 0.0f;
    successfull_deliv_made = 0;
    deliv_yet_to_be_complete = 0;
  }
  int addToDeliveryList(int cId){
    if(successfull_deliv_made>=200){
      if(!cleanUpSize())
      return ERROR;
    }
    customer_id[deliv_yet_to_be_complete] = cId;
    return deliv_yet_to_be_complete++;
  }
  int removeFromDeliveryList(int cId){
    for (int i = successfull_deliv_made+deliv_yet_to_be_complete; i >= successfull_deliv_made; i--) {
      if(customer_id[i]==cId){
        for (int j = i; j < successfull_deliv_made+deliv_yet_to_be_complete-1; j++) {
          int *temp=&customer_id[j];
          customer_id[j]=customer_id[j+1];
          customer_id[j+1]=*temp;
        }
        deliv_yet_to_be_complete--;
        return i;
      }
    }
    return ERROR;
  }
  int makeDelivery(){
    if(deliv_yet_to_be_complete  != 0){
      successfull_deliv_made++;
      deliv_yet_to_be_complete--;
      updateEarning(getEarning());
      return deliv_yet_to_be_complete;
    }
    return ERROR;
  }
  void updateEarning(Money amt){
    earned+=amt;
  }
};


ID bId=0;
class Bill {
private:
  ID id;
  Customer *c;
  Cart *ct;
  Money *total;
  int *status;
protected:
  Customer customer_dereference(){
    return *c;
  }
  Cart cart_dereference(){
    return *ct;
  }
  Money amt_dereference(){
    return *total;
  }
public:
  Bill(Customer c1,Cart c,Money m,int s){
    id=bId++;
    ct=&c;
    total = &m;
    status = &s;
  }
  int getID(){
    return id;
  }
  friend std::ostream  &operator<<( std::ostream &output, Bill &D ){
    std::cout << D.id<<'\t'<<D.customer_dereference()<<D.cart_dereference()<<'\t'<<D.amt_dereference() << '\n';
    return output;
  }
};


//Recoders are maintained in respective struct
typedef struct customer_struct {
  unsigned int slno;
  Customer foo;
  customer_struct *NEXT;
}CRecord;

typedef struct product_struct {
  unsigned int slno;
  Product foo;
  product_struct *NEXT;
}PRecord;

typedef struct wearhouse_struct {
  unsigned int slno;
  WearHouse foo;
  wearhouse_struct *NEXT;
}WRecord;

typedef struct bill_struct {
  unsigned int slno;
  Bill foo;
  bill_struct *NEXT;
}BRecord;

char init() {
  char choice;
  std::cout << "1 - Add customer\n2 - Customer login\n3 - Admin login" << '\n';
  std::cin >> choice;
  return choice ;
}
void create_customer(CRecord *customer) {
  char ad[100];
  char name[20];
  char email_id[25];
  std::cout << "Name : " << '\n';
  scanf("%20s",name );
  std::cout << "Email" << '\n';
  scanf("%25s",email_id );
  std::cout << "Address" << '\n';
  scanf("%100s",ad );
  Customer temp(ad,name,email_id);
  std::cout <<"Welcome "<< temp << '\n';

}
int main(int argc, char const *argv[]) {
  CRecord *customer = NULL;
  switch (init()) {
    case '1':
      create_customer(customer);
      break;
    default:
      std::cout << "Under Construction\nWith caring \n-S.Chandrakumar" << '\n';

  }
  return 0;
}
