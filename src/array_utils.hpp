#pragma once

#include <TColgp_Array1OfPnt2d.hxx>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <TColStd_Array2OfReal.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <gp_Pnt.hxx>

// Handle array types for interpolation APIs
#include <TColgp_HArray1OfPnt.hxx>
#include <TColgp_HArray1OfPnt2d.hxx>
#include <TColgp_Array1OfVec.hxx>
#include <TColgp_Array1OfVec2d.hxx>
#include <TColStd_HArray1OfReal.hxx>
#include <TColStd_HArray1OfBoolean.hxx>
#include <gp_Vec.hxx>
#include <gp_Vec2d.hxx>
#include <gp_Pnt2d.hxx>

namespace {
    namespace py = pybind11;
    // Utility functions for container conversion
    template <typename T, typename ArrayType>
    ArrayType py_container_to_occt_array(const py::handle &container) {
    // Check if it's a numpy array (any numeric dtype)
    if (py::isinstance<py::array>(container)) {
        // Cast to the target dtype to handle int64->int32, float32->float64, etc.
        auto arr = py::array_t<T>::ensure(container);
        if (!arr) {
            throw py::type_error("Cannot convert numpy array to the required numeric type");
        }
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
        if (py::isinstance<py::array>(container)) {
            auto arr = py::array_t<double>::ensure(container);
            if (!arr) {
                throw py::type_error("Cannot convert numpy array to double");
            }
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

    // Helper: convert Python container to TColgp_Array1OfPnt2d
    inline TColgp_Array1OfPnt2d py_container_to_pnt2d_array(const py::handle& container) {
        if (py::isinstance<py::list>(container) || py::isinstance<py::tuple>(container)) {
            py::sequence seq = py::reinterpret_borrow<py::sequence>(container);
            int n = static_cast<int>(py::len(seq));
            TColgp_Array1OfPnt2d arr(1, n);
            for (int i = 0; i < n; ++i) {
                py::object item = seq[i];
                if (py::isinstance<gp_Pnt2d>(item)) {
                    arr.SetValue(i + 1, item.cast<gp_Pnt2d>());
                } else {
                    py::sequence pt = item.cast<py::sequence>();
                    arr.SetValue(i + 1, gp_Pnt2d(pt[0].cast<double>(), pt[1].cast<double>()));
                }
            }
            return arr;
        } else if (py::isinstance<py::array>(container)) {
            auto np_arr = py::array_t<double>::ensure(container);
            if (!np_arr) {
                throw py::type_error("Cannot convert numpy array to double");
            }
            if (np_arr.ndim() != 2 || np_arr.shape(1) != 2) {
                throw py::value_error("NumPy array must have shape (n, 2)");
            }
            int n = static_cast<int>(np_arr.shape(0));
            TColgp_Array1OfPnt2d arr(1, n);
            auto ptr = np_arr.data();
            for (int i = 0; i < n; ++i) {
                arr.SetValue(i + 1, gp_Pnt2d(ptr[i * 2], ptr[i * 2 + 1]));
            }
            return arr;
        }
        throw py::type_error("Expected list, tuple, or numpy array of 2D points");
    }

    template <typename T, typename ArrayType>
    auto occt_array_to_numpy(const ArrayType &array) {
        int size = array.Upper() - array.Lower() + 1;
        auto result = py::array_t<T>(size);
        auto ptr = result.mutable_data();
        for (int i = 0; i < size; i++) {
            ptr[i] = static_cast<T>(array.Value(array.Lower() + i));
        }
        return result;
    }
    auto occt_array_to_numpy(const TColStd_Array1OfReal &array) {
        return occt_array_to_numpy<double, TColStd_Array1OfReal>(array);
    }

    auto occt_array_to_numpy(const TColStd_Array1OfInteger &array) {
        return occt_array_to_numpy<int, TColStd_Array1OfInteger>(array);
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

    inline py::array_t<double> pnt_array_to_numpy(const TColgp_Array1OfPnt2d &array) {
        int size = array.Upper() - array.Lower() + 1;
        auto result = py::array_t<double>({size, 2});
        auto ptr = result.mutable_data();
        for (int i = 0; i < size; i++) {
            const gp_Pnt2d &pnt = array.Value(array.Lower() + i);
            ptr[i * 2] = pnt.X();
            ptr[i * 2 + 1] = pnt.Y();
        }
        return result;
    }

    // ========== 2D Array utilities for surfaces ==========

    // Convert numpy array to 2D OCCT point array
    inline TColgp_Array2OfPnt py_array_to_pnt_array2(const py::array_t<double>& arr) {
        auto buf = arr.request();
        if (buf.ndim != 3 || buf.shape[2] != 3) {
            throw py::value_error("Expected numpy array of shape (nu, nv, 3)");
        }
        int nu = static_cast<int>(buf.shape[0]);
        int nv = static_cast<int>(buf.shape[1]);
        TColgp_Array2OfPnt result(1, nu, 1, nv);
        double* ptr = static_cast<double*>(buf.ptr);
        for (int i = 0; i < nu; ++i) {
            for (int j = 0; j < nv; ++j) {
                int idx = (i * nv + j) * 3;
                result.SetValue(i + 1, j + 1, gp_Pnt(ptr[idx], ptr[idx + 1], ptr[idx + 2]));
            }
        }
        return result;
    }

    // Convert 2D OCCT point array to numpy
    inline py::array_t<double> pnt_array2_to_numpy(const TColgp_Array2OfPnt& array) {
        int nu = array.ColLength();
        int nv = array.RowLength();
        py::array_t<double> result({nu, nv, 3});
        auto buf = result.mutable_unchecked<3>();
        for (int i = 1; i <= nu; ++i) {
            for (int j = 1; j <= nv; ++j) {
                const gp_Pnt& p = array(i, j);
                buf(i - 1, j - 1, 0) = p.X();
                buf(i - 1, j - 1, 1) = p.Y();
                buf(i - 1, j - 1, 2) = p.Z();
            }
        }
        return result;
    }

    // Convert numpy array to 2D OCCT real array
    inline TColStd_Array2OfReal py_array_to_real_array2(const py::array_t<double>& arr) {
        auto buf = arr.request();
        if (buf.ndim != 2) {
            throw py::value_error("Expected 2D numpy array");
        }
        int nu = static_cast<int>(buf.shape[0]);
        int nv = static_cast<int>(buf.shape[1]);
        TColStd_Array2OfReal result(1, nu, 1, nv);
        double* ptr = static_cast<double*>(buf.ptr);
        for (int i = 0; i < nu; ++i) {
            for (int j = 0; j < nv; ++j) {
                result.SetValue(i + 1, j + 1, ptr[i * nv + j]);
            }
        }
        return result;
    }

    // Convert 2D OCCT real array to numpy
    inline py::array_t<double> real_array2_to_numpy(const TColStd_Array2OfReal& array) {
        int nu = array.ColLength();
        int nv = array.RowLength();
        py::array_t<double> result({nu, nv});
        auto buf = result.mutable_unchecked<2>();
        for (int i = 1; i <= nu; ++i) {
            for (int j = 1; j <= nv; ++j) {
                buf(i - 1, j - 1) = array(i, j);
            }
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

    template<typename T>
    auto occt_container_to_py_list(const T& container) {
        auto py_list = py::list();
        for (const auto& item : container) {
            py_list.append(item);
        }
        return py_list;
    };

    // ========== Handle Array (HArray) utilities for interpolation APIs ==========

    // Convert vector of gp_Pnt to Handle(TColgp_HArray1OfPnt)
    inline Handle(TColgp_HArray1OfPnt) vector_to_HArray1OfPnt(const std::vector<gp_Pnt>& points) {
        if (points.empty()) {
            throw std::runtime_error("Points array cannot be empty");
        }
        Handle(TColgp_HArray1OfPnt) arr = new TColgp_HArray1OfPnt(1, static_cast<Standard_Integer>(points.size()));
        for (size_t i = 0; i < points.size(); ++i) {
            arr->SetValue(static_cast<Standard_Integer>(i + 1), points[i]);
        }
        return arr;
    }

    // Convert vector of gp_Pnt2d to Handle(TColgp_HArray1OfPnt2d)
    inline Handle(TColgp_HArray1OfPnt2d) vector_to_HArray1OfPnt2d(const std::vector<gp_Pnt2d>& points) {
        if (points.empty()) {
            throw std::runtime_error("Points array cannot be empty");
        }
        Handle(TColgp_HArray1OfPnt2d) arr = new TColgp_HArray1OfPnt2d(1, static_cast<Standard_Integer>(points.size()));
        for (size_t i = 0; i < points.size(); ++i) {
            arr->SetValue(static_cast<Standard_Integer>(i + 1), points[i]);
        }
        return arr;
    }

    // Convert vector of doubles to Handle(TColStd_HArray1OfReal)
    inline Handle(TColStd_HArray1OfReal) vector_to_HArray1OfReal(const std::vector<double>& values) {
        if (values.empty()) {
            throw std::runtime_error("Values array cannot be empty");
        }
        Handle(TColStd_HArray1OfReal) arr = new TColStd_HArray1OfReal(1, static_cast<Standard_Integer>(values.size()));
        for (size_t i = 0; i < values.size(); ++i) {
            arr->SetValue(static_cast<Standard_Integer>(i + 1), values[i]);
        }
        return arr;
    }

    // Convert vector of bools to Handle(TColStd_HArray1OfBoolean)
    inline Handle(TColStd_HArray1OfBoolean) vector_to_HArray1OfBoolean(const std::vector<bool>& values) {
        if (values.empty()) {
            throw std::runtime_error("Values array cannot be empty");
        }
        Handle(TColStd_HArray1OfBoolean) arr = new TColStd_HArray1OfBoolean(1, static_cast<Standard_Integer>(values.size()));
        for (size_t i = 0; i < values.size(); ++i) {
            arr->SetValue(static_cast<Standard_Integer>(i + 1), values[i] ? Standard_True : Standard_False);
        }
        return arr;
    }

    // Convert vector of gp_Vec to TColgp_Array1OfVec
    inline TColgp_Array1OfVec vector_to_Array1OfVec(const std::vector<gp_Vec>& vecs) {
        if (vecs.empty()) {
            throw std::runtime_error("Vectors array cannot be empty");
        }
        TColgp_Array1OfVec arr(1, static_cast<Standard_Integer>(vecs.size()));
        for (size_t i = 0; i < vecs.size(); ++i) {
            arr.SetValue(static_cast<Standard_Integer>(i + 1), vecs[i]);
        }
        return arr;
    }

    // Convert vector of gp_Vec2d to TColgp_Array1OfVec2d
    inline TColgp_Array1OfVec2d vector_to_Array1OfVec2d(const std::vector<gp_Vec2d>& vecs) {
        if (vecs.empty()) {
            throw std::runtime_error("Vectors array cannot be empty");
        }
        TColgp_Array1OfVec2d arr(1, static_cast<Standard_Integer>(vecs.size()));
        for (size_t i = 0; i < vecs.size(); ++i) {
            arr.SetValue(static_cast<Standard_Integer>(i + 1), vecs[i]);
        }
        return arr;
    }

    // Convert vector of gp_Pnt to TColgp_Array1OfPnt
    inline TColgp_Array1OfPnt vector_to_Array1OfPnt(const std::vector<gp_Pnt>& points) {
        if (points.empty()) {
            throw std::runtime_error("Points array cannot be empty");
        }
        TColgp_Array1OfPnt arr(1, static_cast<Standard_Integer>(points.size()));
        for (size_t i = 0; i < points.size(); ++i) {
            arr.SetValue(static_cast<Standard_Integer>(i + 1), points[i]);
        }
        return arr;
    }

    // Convert vector of gp_Pnt2d to TColgp_Array1OfPnt2d
    inline TColgp_Array1OfPnt2d vector_to_Array1OfPnt2d(const std::vector<gp_Pnt2d>& points) {
        if (points.empty()) {
            throw std::runtime_error("Points array cannot be empty");
        }
        TColgp_Array1OfPnt2d arr(1, static_cast<Standard_Integer>(points.size()));
        for (size_t i = 0; i < points.size(); ++i) {
            arr.SetValue(static_cast<Standard_Integer>(i + 1), points[i]);
        }
        return arr;
    }

    // Convert vector of doubles to TColStd_Array1OfReal
    inline TColStd_Array1OfReal vector_to_Array1OfReal(const std::vector<double>& values) {
        if (values.empty()) {
            throw std::runtime_error("Values array cannot be empty");
        }
        TColStd_Array1OfReal arr(1, static_cast<Standard_Integer>(values.size()));
        for (size_t i = 0; i < values.size(); ++i) {
            arr.SetValue(static_cast<Standard_Integer>(i + 1), values[i]);
        }
        return arr;
    }
} // namespace