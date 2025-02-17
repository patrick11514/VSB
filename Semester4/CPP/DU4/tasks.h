#pragma once

#include <optional>
#include <string>
#include <exception>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ostream>

/**
   ____      _                          _ _                  ____  _            _
  / __ \    | |                        | (_)                / __ \| |          | |
 | |  | | __| |_ __   _____   _____  __| |_   _ __   __ _  | |  | | |_ __ _ ___| | ___   _
 | |  | |/ _` | '_ \ / _ \ \ / / _ \/ _` | | | '_ \ / _` | | |  | | __/ _` |_  / |/ / | | |
 | |__| | (_| | |_) | (_) \ V /  __/ (_| | | | | | | (_| | | |__| | || (_| |/ /|   <| |_| |
  \____/ \__,_| .__/ \___/ \_/ \___|\__,_|_| |_| |_|\__,_|  \____/ \__\__,_/___|_|\_\\__, |
              | |                                                                     __/ |
              |_|                                                                    |___/

[*] Can individual elements be shared among different JSON containers (array/object) with the mentioned
design? Can you take one specific element (e.g. an integer) allocated on the heap and place it inside
an array and also inside an object?

Technicky proč ne, ale mohlo by dojít:
1. k rekurzi, tedy třeba v Objektu bude někde vložen odkaz na sám sebe
2. data můžou být z venku smazána, tedy při čtení nemusí daný integer existovat


[*] Could references (or non-owned pointers) be used for storing the elements in JSON containers?
How would it affect the usability of arrays and objects? Try it and see how easy or hard it is :)

Jak je zmíněno výše, tak se poté může stát, že již daný element nebude existovat a reference tedy bude neplatná.


[*] Think about this interface. What are its benefits or disadvantages? Is it better to put the
indexers into the root `Value` element? Or should it only be supported by types that actually
implement indexing (arrays/objects)? Think about the trade-offs (compile-time safety vs ergonomics).

Z ergonomického hlediska nemusíme přetypovávát na správný typ, abychom mohli operátor zavolat a můžeme ho rovnou zavolat
na Value. Během compile timu zase nemusí kompilátor kontrolovat, zda daný typ má tuto metodu, ale zase tyto metody jsou atomaticky v poděděných
třídách a musí zde zapojit vtable na zavolání správné metody.


[*] Think about the return type. What should it be? Is `std::optional` required here?

V našem případě se std::optional používat nemusí, protože můžeme použít jako návratový typ Value*, ale kdybychom
implementovali například string, kde by se pomocí operatoru [] indexovaly například jeho charaktery, tak si osobně myslím
že již nebude možné to dělat přes overridování metody.


[*] How can you implement copying for a polymorphic object hierarchy? What should be the return type
of the `clone` method? If you are interested, look for "C++ covariance".

-, Return type clone metody bude Value*, s tím že při overridu si můžu dovolit
returnovat například Array* atd.. protože se jedná o potomka Value


[*] Think about the constness of the `accept` method and of the methods in the visitor. What
parameter type should they take? Should it be const or not? Consider creating two classes
to represent visitors, `Visitor` and `MutatingVisitor` (or similar names).

U komplexnějších struktur se poté může stát, že se accept metoda volá v accept metoddě a ta v accept metodě...
a teoreticky může dojít k stack overflow. Dále se musí po přidání nového potomka třídy přidat korespondující metoda
do Visitora. Metoda visitora pokud nemá v plánu měnit nějak data třídy by měla brát konstatní referenci, pokud ale má
v plánu volat metody, které mění data, tak poté by měla brát nekonstantní referenci.
Nevýhodou tady také je, že nemůžeme přímo sahat na privátní/protected parametry třídy a můžeme pouze upravovat data
přes poskytnuté metody.


[*] How would the implementation look like if you have used e.g. algebraic data types instead?
Would you need the Visitor design pattern in that case?

-
*/

class Visitor;
class MutatingVisitor;

class Value
{
public:
    virtual ~Value() = default;

    virtual Value *operator[](const size_t) const;
    virtual Value *operator[](const std::string &) const;
    virtual Value *clone() const;
    virtual void accept(const Visitor &visitor) const;
    virtual void accept(const MutatingVisitor &visitor);
};

class Integer : public Value
{
private:
    int value;

public:
    Integer(const int value);
    ~Integer() = default;

    int get_value() const;
    Integer *clone() const;
    void accept(const Visitor &visitor) const final;
    void accept(const MutatingVisitor &visitor) final;
};

class Null : public Value
{
    void accept(const Visitor &visitor) const final;
    void accept(const MutatingVisitor &visitor) final;
};

class Array : public Value
{
private:
    std::vector<Value *> value;

public:
    Array() = default;
    Array(std::vector<Value *> values);
    ~Array();

    void append(Value *value);
    void remove(const size_t index);

    size_t size() const;
    Value *operator[](const size_t index) const final;
    Array *clone() const;
    void accept(const Visitor &visitor) const final;
    void accept(const MutatingVisitor &visitor) final;
};

class Object : public Value
{
private:
    std::unordered_map<std::string, Value *> value;

public:
    Object() = default;
    // https://upload.patrick115.eu/screenshot/ksnip_tmp_GCLyGf.png
    // Object(const std::vector<std::pair<std::string, Value *>> &values);
    Object(std::unordered_map<std::string, Value *> map);
    ~Object();

    void insert(const std::string &key, Value *value);
    void remove(const std::string &key);

    size_t size() const;
    std::vector<std::string> keys() const;
    Value *operator[](const std::string &key) const final;
    Object *clone() const;
    void accept(const Visitor &visitor) const final;
    void accept(const MutatingVisitor &visitor) final;
};

class UnIndexableException : public std::exception
{
public:
    char *what();
};

class Visitor
{
public:
    virtual void visitValue(const Value &value) const;
    virtual void visitInteger(const Integer &value) const;
    virtual void visitNull(const Null &value) const;
    virtual void visitArray(const Array &value) const;
    virtual void visitObject(const Object &value) const;
};

class MutatingVisitor
{
public:
    virtual void visitValue(Value &value) const;
    virtual void visitInteger(Integer &value) const;
    virtual void visitNull(Null &value) const;
    virtual void visitArray(Array &value) const;
    virtual void visitObject(Object &value) const;
};

class RemoveNullVisitor : public MutatingVisitor
{
    void visitArray(Array &value) const final;
    void visitObject(Object &value) const final;
};

class PrintVisitor : public Visitor
{
private:
    std::ostream &stream;

public:
    PrintVisitor(std::ostream &stream);

    virtual void visitInteger(const Integer &value) const final;
    virtual void visitNull(const Null &) const final;
    virtual void visitArray(const Array &value) const final;
    virtual void visitObject(const Object &value) const final;
};