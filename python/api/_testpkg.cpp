#include <boost/python.hpp>

#include <memory>
#include <iostream>
#include <string>

namespace py = boost::python;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;

class Data {
    public:
        Data(int val) : number(val) {}
        int getset(int val) {
            int result = number;
            number = val;
            cout << "Data value was " << result << " and is now " << number << endl;
            return result;
        }
    private:
        int number;
};

void printData(Data d) { d.getset(5); }
void printDataPtr(Data *d) { d->getset(4); }
void printDataShrdPtr(const shared_ptr<Data> &d) { d->getset(3); }

class Interface {
    public:
        virtual ~Interface(){}
        virtual void function(int arg) {
            cout << "Default implementation" << endl;
            cout << "value of arg is " << arg << endl;
        }
};

void printIntPtr(Interface *ptr) { ptr->function(5); }
void printIntShPtr(const shared_ptr<Interface> &i) { i->function(7); }

class InterfaceWrapper : public Interface, public py::wrapper<Interface>
{
    public:
    void function(int arg) {
        // Check for override in Python
        if(py::override f = this->get_override("function")) {
            cout << "Found Python override for 'void function(int)'" << endl;
            f(arg);
            return;
        }

        // Or call C++ implementation
        cout << "No Python override found, calling C++ implementation." << endl;
        return Interface::function(arg);
    }

    void default_function(int arg) {
        return this->Interface::function(arg);
    }
};

BOOST_PYTHON_MODULE(_testpkg)
{
    py::class_< Data, shared_ptr<Data> >("Data", py::init<int>())
           .def("getset", &Data::getset);

    py::def("print_data", printData);
    py::def("print_data_ptr", printDataPtr);
    py::def("print_data_shptr", printDataShrdPtr);

    py::class_<InterfaceWrapper, std::shared_ptr<InterfaceWrapper>, boost::noncopyable>("Interface")
        .def("function", &Interface::function, &InterfaceWrapper::default_function);

    py::def("print_i_ptr", printIntPtr);
    py::def("print_i_shptr", printIntShPtr);

    py::implicitly_convertible< std::shared_ptr<InterfaceWrapper>, std::shared_ptr<Interface> >();
}
