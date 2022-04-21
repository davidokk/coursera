#include <iostream>
#include <vector>

using namespace std;

class Matrix {
private:
    vector <vector <int>> matrix;
public:
    Matrix() {}
    Matrix(int num_rows, int num_cols) {
        if (num_rows < 0 || num_cols < 0)
            throw out_of_range("");
        if (num_rows == 0 || num_cols == 0)
            num_cols = num_rows = 0;
        matrix.resize(num_rows);
        for (auto& r : matrix)
            r.resize(num_cols);
    }
    void Reset(int num_rows, int num_cols) {
        if (num_rows < 0 || num_cols < 0)
            throw out_of_range("");
        if (num_rows == 0 || num_cols == 0)
            num_cols = num_rows = 0;
        matrix.resize(num_rows);
        for (auto& r : matrix)
            r.resize(num_cols);
        for (auto& r : matrix)
            for (auto& i : r)
                i = 0;
    }
    int At(int num_row, int num_col) const {
        if (num_row >= GetNumRows() || num_col >= GetNumColumns() || num_col < 0 || num_row < 0)
            throw out_of_range("");
        return matrix[num_row][num_col];
    }
    int& At(int num_row, int num_col) {
        if (num_row >= GetNumRows() || num_col >= GetNumColumns() || num_col < 0 || num_row < 0)
            throw out_of_range("");
        return matrix[num_row][num_col];
    }
    int GetNumRows() const {
        return matrix.size();
    }
    int GetNumColumns() const {
        if (!matrix.empty())
            return matrix[0].size();
        return 0;
    }

    friend istream& operator>> (istream& is, Matrix& a) {
        int n, m;
        is >> n >> m;
        a = Matrix(n, m);
        for (auto& r : a.matrix)
            for (auto& i : r)
                is >> i;
        return is;
    }

    friend ostream& operator<< (ostream& os, const Matrix& a) {
        os << a.GetNumRows() << " " << a.GetNumColumns() << endl;
        for (auto &r : a.matrix) {
            for (auto &i : r)
                os << i << " ";
            os << endl;
        }
        return os;
    }

    bool operator== (const Matrix& m) const {
        if (GetNumColumns() != m.GetNumColumns() || GetNumRows() != m.GetNumRows())
            return false;
        for (int i = 0; i < GetNumRows(); i++)
            for (int j = 0; j < GetNumColumns(); j++)
                if (matrix[i][j] != m.matrix[i][j])
                    return false;
        return true;
    }

    Matrix operator+ (const Matrix& m) const {
        if (GetNumRows() != m.GetNumRows() || GetNumColumns() != m.GetNumColumns())
            throw invalid_argument("");
        Matrix new_matrix = Matrix(GetNumRows(), GetNumColumns());
        for (int i = 0; i < GetNumRows(); i++)
            for (int j = 0; j < GetNumColumns(); j++)
                new_matrix.matrix[i][j] = matrix[i][j] + m.matrix[i][j];
        return new_matrix;
    }
};

int main() {
    Matrix a, b;
    cin >> a >> b;
    cout << a + b;
    return 0;
}
