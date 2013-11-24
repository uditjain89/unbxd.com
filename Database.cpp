#include <tr1/unordered_map>
#include<string>
#include<iostream>
using namespace std::tr1 ;
using namespace std ;

class Product ;
class Record ;
class ProductList
{
  public:
  Product* prod ;
  ProductList* next ;
};

class RecordList
{
  public:
  Record* rec ;
  RecordList* next ;

};
 


class Database
{
   // hash of product id vs product
   unordered_map< int , Product* > _idToProduct ;

   // hash of artist vs list of products
  unordered_map< string, ProductList* > _artistToProducts ;

   // hash of query vs hash of productIds to list of records  
  unordered_map< string , unordered_map<int, RecordList*>*  >  _queryToProdHash ;
  
  // hash of product ids vs list of all the records.
  unordered_map< int , RecordList* > _idToRecordList ;
 
  public:
    Database() ;
   // ~Database() ;
    void addProduct( int , string& , string& , string& ) ;
    void createQueryRecord( string& , int , string& ) ;
    void productsQueried( string& ) ;
    void queryForArtist( string& ) ;
};



class Product
{
  public:
  int _productId ;
  string _productName ;
  string _genre ;
  string _artist ;

  public:
  
  Product( int, string& , string& , string& ) ;

};

class Record
{
  public:
  string _query ;
  int _productId ;
  string _timestamp ;
  public:

  Record ( string& , int , string& ) ;

};

Product::Product( int id, string& name, string& genre, string& artist ) 
{
  _productId = id ;
  _productName = name ;
  _genre = genre ;
  _artist = artist ;
}

Record::Record( string& query , int id , string& time)
{
  _query = query ;
  _productId = id ;
  _timestamp = time ;
}


Database::Database( )
{
  _idToProduct.clear() ;

   // hash of artist vs list of products
  _artistToProducts.clear() ;

   // hash of query vs hash of productIds to list of records  
  _queryToProdHash.clear() ;
  
  // hash of product ids vs list of all the records.
  _idToRecordList.clear() ;

}

void Database::addProduct( int productId, string& name, string& genre, string& artist ) 
{
   

   // TODO add <productId , obj > pair in a hash table .
   if( !( _idToProduct.find( productId ) == _idToProduct.end() ) )
   {
     cout << "Duplicate Product..."<< productId << endl ; 
     return ;
   }
   else
   {
     Product* obj = new Product( productId, name , genre , artist ) ;
     _idToProduct.insert( pair<int,Product*>( productId, obj ) ) ;

     // find artist in hash of list of products, if list found , create a new node and add it as the second node of the list,
     // else create a new list ,  add it  in the hash
     unordered_map<string, ProductList*>::const_iterator iter ;
     ProductList* list = NULL ;
     ProductList* node = new ProductList ;
     node->prod = obj ;
     node->next = NULL ;
     if( (iter = _artistToProducts.find( artist )) == _artistToProducts.end() )
     {
       list = node ;
      // list->prod = obj ;
      // list->next = NULL ;
       _artistToProducts.insert( pair<string,ProductList*>( artist, list) ) ;
     }
     else
     {
       // add current product to the list of products of this artist.
       list = iter->second ;
       ProductList* temp = list->next ;
       list->next = node ;
       node->next = temp ;
     }
     
   }
   
    
}

void Database::createQueryRecord( string& query , int productId, string& time ) 
{
  if( _idToProduct.find( productId ) == _idToProduct.end() )
  {
    cout <<"InValid record. No Such Product " << endl ;
    return ;
  }
  Record* rec = new Record( query , productId ,time ) ;
  
  // get list of recs for productId, if exists ,add rec ,else create a new list and add in table
  unordered_map<int, RecordList*>::const_iterator iter ;
  RecordList* list = NULL ;
  RecordList* node = new RecordList ;
  node->rec = rec ;
  node->next = NULL ;
  if( (iter = _idToRecordList.find( productId) ) == _idToRecordList.end() )
  {
    list = node ;
    _idToRecordList.insert( pair<int,RecordList*>( productId,list) ) ;
  } 
  else
  {
    list = iter->second ;
    RecordList* temp = list->next ;
    list->next = node ;
    node->next = temp ;
  }
  
  // get hash of product id vs list of records for this query.
  list = NULL ;
  RecordList* otherNode = new RecordList ;
  otherNode->rec = rec ;
  otherNode->next = NULL ;
  unordered_map<int , RecordList*>* idMap = NULL ;
  unordered_map<string, unordered_map<int , RecordList*>* >::const_iterator it ;
  if( (it= _queryToProdHash.find( query ) ) == _queryToProdHash.end() )
  {
    idMap = new unordered_map<int , RecordList*> ;
    idMap->insert(pair<int,RecordList*>( productId, otherNode ) ) ; 
    _queryToProdHash.insert( pair<string, unordered_map<int , RecordList*>* >( query , idMap ) ) ;
  }
  else
  {
    idMap = it->second ;
    if( (iter = idMap->find( productId ) ) == idMap->end() )
    {
       idMap->insert( pair<int,RecordList*>( productId, otherNode) ) ;
    }
    else
    {
      list = iter->second; 
      RecordList* temp = list->next ;
      list->next = otherNode ;
      otherNode->next = temp ;     
    }
    
  }
  
}

void Database::productsQueried( string& query ) 
{
  unordered_map< string, unordered_map<int,RecordList*>* >::const_iterator iter ;
  unordered_map<int,RecordList*>* idMap = NULL ;
  if( (iter = _queryToProdHash.find( query ) ) == _queryToProdHash.end() )
  {
    cout <<"No Such Query Made " << endl ;
    return ;
  }
  idMap = iter->second ;
  unordered_map<int,RecordList*>::iterator it ;
  unordered_map<int,Product*>::iterator prodIter ;
  it = idMap->begin() ;
  while( it != idMap->end() )
  {
    int id = it->first ;
    Product* prod = NULL ;
    
    if( (prodIter = _idToProduct.find( id) )== _idToProduct.end() )
    {
      cout<<" Some Error " << endl ;
    }
    prod = prodIter->second ;
    // print all products attibutes .. id , name , genre , artist
    cout <<"ID : " << id << "  , Name :  " << prod->_productName <<"  , Genre : " << prod->_genre <<"   , Artist : "<< prod->_artist << endl << endl;
    it++ ;
  }

}


void Database::queryForArtist( string& artist )
{
  ProductList* prodList = NULL ;
  RecordList* recList = NULL ;
  unordered_map<string,ProductList*>::const_iterator iter;
  unordered_map<int,RecordList*>::iterator recordIter ;
  if( (iter = _artistToProducts.find(artist)) == _artistToProducts.end() )
  { 
    cout<< "No Such Artist " << endl << endl ;
    return ;
  }
  Product* prod = NULL ;
  Record* rec = NULL ;
  prodList = iter->second ;
  while( prodList )
  {
    prod = prodList->prod ;
    int id = prod->_productId ;
    if( (recordIter = _idToRecordList.find( id ) ) != _idToRecordList.end() )
    {
      recList = recordIter->second ;
      while( recList )
      {
        rec = recList->rec ;
        cout <<"Query : " << rec->_query <<"  , time :  "<< rec->_timestamp << endl<< endl ;
        recList = recList->next ;
      }
    } 
    prodList = prodList->next ;    
  }
}

