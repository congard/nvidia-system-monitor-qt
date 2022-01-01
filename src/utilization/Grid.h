#ifndef QNVSM_GRID_H
#define QNVSM_GRID_H

class Grid {
public:
    inline explicit Grid(int rows)
        : m_rows(rows) {}

    inline Grid& operator++() {
        ++m_row;
        m_col += m_row / m_rows;
        m_row %= m_rows;
        return *this;
    }

    inline int row() const {
        return m_row;
    }

    inline int column() const {
        return m_col;
    }

private:
    int m_rows;
    int m_row {0};
    int m_col {0};
};

#endif //QNVSM_GRID_H
