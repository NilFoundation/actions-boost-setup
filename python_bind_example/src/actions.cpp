#ifndef PYTHON_BINDINGS_PYTHON_BINDINGS_HPP_
#define PYTHON_BINDINGS_PYTHON_BINDINGS_HPP_

#include <boost/python.hpp>

using namespace boost::python;

std::string greet()
{
  return "Hello, world with full cache!";
}

BOOST_PYTHON_MODULE(actions)
{
    def("greet", greet);
}

#endif    // PYTHON_BINDINGS_PYTHON_BINDINGS_HPP_
