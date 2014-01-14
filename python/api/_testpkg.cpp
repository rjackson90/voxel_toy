#include <boost/python.hpp>

#include <memory>
#include <iostream>
#include <string>

namespace py = boost::python;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;

class Data
{
    int value;

    public:
    Data(int x) : value(x){}
    
    void doSomething(){ value += 1; }
    int getVal() { return value; }
};

class Information : public Data
{
    string info;

    public:
    Information(const string& val, int data) : Data(data), info(val){}
};

void printData(const shared_ptr<Data>& data)
{
    data->doSomething();
    cout << data->getVal() << endl;
}

class Container
{
    shared_ptr<Data> value;

    public:
    Container(const shared_ptr<Data>& data) : value(data){}
    string print(){ return string("There's data in the box!"); }
};

BOOST_PYTHON_MODULE(_testpkg)
{
    py::class_<Data, shared_ptr<Data> >("Data", py::init<int>())
        .def("doSomething", &Data::doSomething)
        .def("getVal", &Data::getVal);

    py::class_<Information, shared_ptr<Information>, py::bases<Data>>(
            "Information", py::init<string, int>());

    py::class_<Container>("Container", py::init<const shared_ptr<Data>&>())
        .def("__str__", &Container::print);

    py::def("printData", printData);

    py::implicitly_convertible<shared_ptr<Information>, shared_ptr<Data>>();
}
