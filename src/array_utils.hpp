#pragma once

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>


#include <TColStd_Array1OfReal.hxx>
#include <TColgp_Array1OfPnt.hxx>

namespace {
    namespace py = pybind11;
    // Utility functions for container conversion
    template <typename T, typename ArrayType>
    ArrayType py_container_to_occt_array(const py::handle &container) {
    // Check if it's a numpy array
    if (py::isinstance<py::array_t<T>>(container)) {
        auto arr = container.cast<py::array_t<T>>();
        if (arr.ndim() != 1) {
        throw py::value_error("Only 1D numpy arrays are supported");
        }

        ArrayType result(1, static_cast<int>(arr.size()));
        auto ptr = arr.data();
        for (int i = 0; i < static_cast<int>(arr.size()); i++) {
        result.SetValue(i + 1,
                        static_cast<typename ArrayType::value_type>(ptr[i]));
        }
        return result;
    }
    // Fallback to list handling
    else if (py::isinstance<py::list>(container)) {
        auto py_list = container.cast<py::list>();
        int size = static_cast<int>(py_list.size());
        ArrayType array(1, size);
        for (int i = 0; i < size; i++) {
        array.SetValue(i + 1, py_list[i].cast<T>());
        }
        return array;
    } else {
        throw py::type_error("Expected list or numpy array");
    }
    }

    // Special handling for gp_Pnt arrays (can't use simple template)
    inline TColgp_Array1OfPnt
    py_container_to_pnt_array(const py::handle &container) {
    if (py::isinstance<py::array_t<double>>(container)) {
        auto arr = container.cast<py::array_t<double>>();
        if (arr.ndim() != 2 || arr.shape(1) != 3) {
        throw py::value_error("For points, expected numpy array of shape (n, 3)");
        }

        TColgp_Array1OfPnt result(1, static_cast<int>(arr.shape(0)));
        auto ptr = arr.data();
        for (int i = 0; i < static_cast<int>(arr.shape(0)); i++) {
        gp_Pnt pnt(ptr[i * 3], ptr[i * 3 + 1], ptr[i * 3 + 2]);
        result.SetValue(i + 1, pnt);
        }
        return result;
    } else if (py::isinstance<py::list>(container)) {
        auto py_list = container.cast<py::list>();
        int size = static_cast<int>(py_list.size());
        TColgp_Array1OfPnt array(1, size);
        for (int i = 0; i < size; i++) {
        array.SetValue(i + 1, py_list[i].cast<gp_Pnt>());
        }
        return array;
    } else {
        throw py::type_error(
            "Expected list of gp_Pnt or numpy array of shape (n, 3)");
    }
    }

    template <typename ArrayType>
    py::array_t<double> occt_array_to_numpy(const ArrayType &array) {
    int size = array.Upper() - array.Lower() + 1;
    auto result = py::array_t<double>(size);
    auto ptr = result.mutable_data();
    for (int i = 0; i < size; i++) {
        ptr[i] = static_cast<double>(array.Value(array.Lower() + i));
    }
    return result;
    }

    // Special handling for converting point arrays to numpy
    inline py::array_t<double> pnt_array_to_numpy(const TColgp_Array1OfPnt &array) {
    int size = array.Upper() - array.Lower() + 1;
    auto result = py::array_t<double>({size, 3});
    auto ptr = result.mutable_data();
    for (int i = 0; i < size; i++) {
        const gp_Pnt &pnt = array.Value(array.Lower() + i);
        ptr[i * 3] = pnt.X();
        ptr[i * 3 + 1] = pnt.Y();
        ptr[i * 3 + 2] = pnt.Z();
    }
    return result;
    }

    template <typename ArrayType>
    py::list occt_array_to_py_list(const ArrayType &array) {
    py::list py_list;
    for (int i = array.Lower(); i <= array.Upper(); i++) {
        py_list.append(array.Value(i));
    }
    return py_list;
    }
} // namespace