#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<cstring>
#include<iomanip>
#include<stdio.h>
#include<cstdlib>
#include<bits/stdc++.h>
using namespace std;



enum category_vals {bronze, silver, gold, platinum, diamond};

class customer;

namespace file_handler {
    vector<vector<string>> view_records(string filename){
        string line, word;
        vector<string> row;
        vector<vector<string>> content;
        fstream file(filename,ios::in);
        if(file.is_open()){
            while(getline(file,line)){
                row.clear();
                stringstream str(line);
                while(getline(str,word,','))
                    row.push_back(word);
                content.push_back(row);
            }
            file.close();
            return content;
        }
        else{
            cout<<"couldnt open the file"<<endl;
            return content;
        }
    }
    void update_record(string filename, vector<vector<string>> content){
        fstream fin;
        // truncate file
        remove(filename.c_str());

        fin.open(filename,ios::in|ios::app);   

        for(auto i : content){
            for(auto j : i){
                fin<<j<<",";
            }
            fin<<endl;
        }
    }

    string assign_category_str(float sum)
    {
        string category;
        if(sum < 1000)
        {
            category = "bronze";
        }
        else if(sum < 5000)
        {
            category = "silver";
        }
        else if(sum < 10000)
        {
            category = "gold";
        }
        else if(sum < 50000)
        {
            category = "platinum";
        }
        else
        {
            category = "diamond";
        }
        return category;
    }

    
}

// classes
// - account (base class for employee and customer)
// - employee // try catch block
// - customer (keep sum total of purchase, track last item bought and recommend it)
// - category (name, discount value) // vector 
// - products // friend function linking product and category
// - ads (object shows occasion and discount) // static functions
class account
{
    string name;
    string password;
    public:
    account()
    {
        name.assign("");
        password.assign("");
    }
    account(string n, string pw)
    {
        name.assign(n);
        password.assign(pw);
    }
    string get_pw()
    {
        return password;
    }
    string get_name()
    {
        return name;
    }
};
class category_class;
class product;
class customer: public account
{
    string mail;
    float purchase_sum;
    string last_product;
    public:
    static int count;
    category_vals category;
    customer():account()
    {
        count++;
        purchase_sum = 0;
        mail.assign("");
        last_product.assign("");
        // category.assign("");
        assign_category();
    }
    // changed parameterised constructor to setter because of switch issue
    customer(string n, string pw, float ps, string lp,string m):account(n,pw)
    {
        count++;
        purchase_sum = ps;
        last_product.assign(lp);
        mail.assign(m);
        assign_category();
        //category.assign(c);
    }

    static string return_id()
    {
        return to_string(count);
    }

    string get_last_product()
    {
        return last_product;
    }
    category_vals get_category()
    {
        return category;
    }
    float get_purchase_sum()
    {
        return purchase_sum;
    }
    void update_purchase_sum(float x)
    {
        purchase_sum+=x;
    }
    void update_category(category_vals x)
    {
        category = x;
    }
    void assign_category()
    {
        if(get_purchase_sum() < 1000)
        {
            category = bronze;
        }
        else if(get_purchase_sum() < 5000)
        {
            category = silver;
        }
        else if(get_purchase_sum() < 10000)
        {
            category = gold;
        }
        else if(get_purchase_sum() < 50000)
        {
            category = platinum;
        }
        else
        {
            category = diamond;
        }
    }

    static customer get_customer(string filename, string name)
    {
        // return the customer class with the given name
        vector<vector<string>> records = file_handler::view_records(filename);
        for(auto i : records)
        {
            if(i[1] == name)
            {
                customer c(i[1],i[2],stof(i[4]),i[5],i[3]);
                return c;
            }
        }
        customer c;
        return c;
    }

    friend void calc_discount(customer c, category_class cat, product p);
    ~customer()
    {
        count--;
    }
};

int customer::count = 0;

class category_class
{
    // this stores the discount values for each category 
    static int discount[5];
    public:
    category_vals category_val;
    category_class(category_vals x)
    {
        category_val = x;
    }
    static int get_discount(category_vals x)
    {
        return discount[x];
    }
    friend void calc_discount(customer c, category_class cat, product p);
};

int category_class::discount[5] = {0, 5, 10, 15, 20};




class ads{
    public:
    
    void recommendation(customer c){
        // category_class cat(c.get_category());
        int discount = category_class::get_discount(c.get_category());
        cout<< endl <<discount<<"\% off available for "<<c.get_last_product() << endl <<"would you like to purchase the same product?"<<endl;
        
    }
    vector<string> offers(){
     vector<string> off;
     off.push_back("get free airpods and powerbrick on purchase of iphone devices");
     off.push_back("get samsung powerbrick and galaxy buds on purchase of samsung products");
     return off;
    }
};

class employee: public account
{
    int emp_id;
    public:
    static int emp_count;
    employee():account()
    {
        emp_count++;
        emp_id = emp_count;
    }
    employee(string n, string pw):account(n,pw)
    {
        emp_count++;
        emp_id = emp_count;
    }
   int login(string usn, string pw)
   {
    try
    {
        // if(!strcmp(admin.get_name(),usn) || !strcmp(admin.get_pw(),pw))
        if(!(get_name() == usn) || !(get_pw() == pw))
        {
            // cout<<"error found"<<endl;
            throw 1;
        }
   } 
   catch(int a)
   {
        return 0;
   }
    // cout<<"error not found"<<endl;
   return 1;
   }
};

int employee::emp_count = 0;



class product
{
   int p_id;
   string name;
   float price;
   public:
   static int count;
   product(){
     count++;
    // p_id=count;
    name="";
    price=0.0;
   }
   product(string n,float p){
    count++;
    p_id=count;
    name=n;
    price=p;
   }
   static string return_id()
    {
        return to_string(count);
    }
    string get_pname(){
        return name;
    }
    float get_price(){
        return price;
    }
    static product get_product(string filename, string pname)
    {
        // return the customer class with the given name
        vector<vector<string>> records = file_handler::view_records(filename);
        for(auto i : records)
        {
            if(i[1] == pname)
            {
                product p(i[1],stof(i[2]));
                return p;
            }
        }

        product p;
        cout<<p.get_pname()<<endl;
        return p;
        
    }
    ~product(){
        count--;
    }
};

void calc_discount(customer c, category_class cat, product p)
{
    float discount = cat.get_discount(c.get_category());
    string last_product = c.get_last_product();
    int last_product_price = p.get_price();
    
    float discounted_price = last_product_price - (last_product_price * discount / 100);
    // c.update_category(cat.category_val);
    cout << "The discount price is : " << discounted_price << endl;
}

int product::count = 0;
//global employee for login 
employee admin("admin","123asd");

int main()
{   
    cout << "Welcome to ABD Supermarket" << endl << endl;
    cout << " -- LOGIN -- " << endl << endl;
    char* filep=getenv("CRM_PATH");
    if (!filep)
    {
        cout << "File path is null" << filep << endl;
    }
    cout << filep << endl;
    // char* prod = filep, *cust = filep;
    // cout << prod << endl << cust << endl;
    // strcat(prod, "/products.csv");
    // strcat(cust, "/customers.csv");
    string prod(filep);
    prod = prod + "/products.csv";
    string cust(filep);
    cust = cust + "/customers.csv";
    string username;
    string password;
    int login_status = 0;
    while(login_status == 0)
    {
        cout << "Enter your username" << endl;
        cin >> username;
        cout << "Enter your password" << endl;
        cin >> password;

        login_status = admin.login(username, password);
    }
    cout << "Login Successful" << endl << endl;

   
    vector<vector<string>> records;
    vector<string> offers;
    string name, mail, last_product;
    float purchase_sum,price;
    vector<string> new_record;
    customer c;
    int r;
    product p;
    ads ad;
    category_class cat(bronze);
    while(1){
        cout << " -- MENU -- " << endl << endl;
        cout << "1. Add new customer" << endl;
        cout << "2. Add new product" << endl;
        cout << "3. View customers"<< endl;
        cout << "4. View discounts for customer" << endl;
        cout << "5. View products" << endl;
        cout << "6. Display Advertisements for customer" << endl;
        cout << "7. Exit" << endl;    
        
        int choice;
        cin >> choice;
        cin.ignore(1000,'\n');

        switch(choice)
        {
            case 1: 
                    customer::count++;
                    cout << "Enter name" << endl;
                    getline(cin, name,'\n');
                    cin.ignore(1000,'\n');
                    cout << "Enter mail" << endl;
                    getline(cin, mail);
                    cin.ignore(1000,'\n');
                    cout << "Enter last product purchased" << endl;
                    getline(cin, last_product);
                    cin.ignore(1000,'\n');
                    cout << "Enter purchase sum" << endl;
                    cin >> purchase_sum;
                    cin.ignore(1000,'\n');
                    // cout << typeid(strcat(filep,cust)).name() << endl;

                    // c = customer(name,"",purchase_sum,last_product,mail);
                    records = file_handler::view_records(cust);
                    new_record.push_back(customer::return_id());
                    new_record.push_back(name);
                    new_record.push_back("");
                    new_record.push_back(mail);
                    new_record.push_back(last_product);
                    new_record.push_back(to_string(purchase_sum));
                    new_record.push_back(file_handler::assign_category_str(purchase_sum));
                    records.push_back(new_record);

                    file_handler::update_record(cust, records);
                    new_record.clear();
                    break;
            case 2: 
                    product::count++;
                    cout << "Enter name" << endl;
                    getline(cin, name);
                    cin.ignore(1000,'\n');
                    cout << "Enter price" << endl;
                    cin >> price;
                    // cin.ignore(1000,'\n');
                    // c = customer(name,"",purchase_sum,last_product,mail);
                    records = file_handler::view_records(prod);
                    new_record.push_back(product::return_id());
                    new_record.push_back(name);
                    new_record.push_back(to_string(price));
                    records.push_back(new_record);
                    file_handler::update_record(prod, records);
                    new_record.clear();
                    break;
            case 3: 
                    records = file_handler::view_records(cust);
                    for(auto i : records)
                    {
                        for(auto j : i)
                        {
                            cout << setw(15)<<j << "\t\t";
                        }
                        cout << endl;
                    }
                    break;
            case 4: 
                    cout << "Enter customer name" << endl;
                    getline(cin ,name);
                    cin.ignore(1000,'\n');
                    
                    c = customer::get_customer(cust, name);
                    if(c.get_name() == "")
                    {
                        cout << "Customer not found" << endl;
                        break;
                    }
                    cout << "Enter product name" << endl;
                    getline(cin, last_product);
                    cin.ignore(1000,'\n');
                    p = product::get_product(prod, last_product);
                    if(p.get_price() == 0.0)
                    {
                        cout << "Product not found" << endl;
                        break;
                    }
                    
                    calc_discount(c, cat, p);
                    break;
            case 5: records = file_handler::view_records(prod);
                    for(auto i : records)
                    {
                        for(auto j : i)
                        {
                            cout <<setw(20)<< j << "\t\t\t\t";
                        }
                        cout << endl;
                    } 
                    break;
            case 6: cout << "Enter customer name" << endl;
                    cin >> name;
                    c = customer::get_customer(cust, name);
                    //recommendation
                    ad.recommendation(c);
                    cout<<endl<<endl;
                    offers=ad.offers();
                    r=(rand() % (offers.size() - 0)) + 0;
                    cout<<offers[r]<<endl<<endl<<endl;
                    break;
            case 7: exit(0);
                    break;
            default: break;
        }
        cin.ignore(1000,'\n');
        getchar();
    }
    return 0;
}
