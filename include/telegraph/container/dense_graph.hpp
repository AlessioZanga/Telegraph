#pragma once

#include "../interface/abstract_graph.ipp"

class DenseGraph : public AbstractGraph {
   protected:
    AdjacencyMatrix A;

   public:
    DenseGraph();

    DenseGraph(const DenseGraph &other);

    DenseGraph &operator=(const DenseGraph &other);

    virtual ~DenseGraph();

    DenseGraph(std::size_t n);

    /**
     * @brief Construct a new Dense Graph object from VIDs iterators.
     *
     * Construct a new Dense Graph object from a pair of VIDs iterators.
     * Let *n* be the size of the sequence of VIDs V, each VID in V is assumed
     * to be (1) *unique* and (2) *between 0 and n-1*.
     *
     * The sequence is *not* required to be ordered, *nor* locally stored.
     *
     * @tparam I Iterator typename.
     * @tparam require_iter_value_type(I, VID) Require the iterator value type to be VID.
     * @param begin First iterator.
     * @param end Last iterator.
     */
    template <typename I, require_iter_value_type(I, VID)>
    DenseGraph(const I &begin, const I &end);

    /**
     * @brief Construct a new Dense Graph object from EIDs iterators.
     *
     * Construct a new Dense Graph object from a pair of EIDs iterators.
     * Each EID in the sequence of EIDs is assumed to be *unique*.
     * Let V be the hypothetical sequence of VIDs given the sequence of EIDs.
     * Let *n* be the size of the sequence of VIDs V, each VID in V is assumed
     * to be (1) *unique* and (2) *between 0 and n-1*.
     *
     * The sequence is *not* required to be ordered, *nor* locally stored.
     *
     * @tparam I Iterator typename.
     * @tparam require_iter_value_type(I, EID) Require the iterator value type to be EID.
     * @param begin First iterator.
     * @param end Last iterator.
     */
    template <typename I, require_iter_value_type(I, EID)>
    DenseGraph(const I &begin, const I &end);

    DenseGraph(const AdjacencyList &other);

    DenseGraph(const AdjacencyMatrix &other);

    DenseGraph(const SparseAdjacencyMatrix &other);

    inline virtual AdjacencyList adjacency_list() const override;

    inline virtual AdjacencyMatrix adjacency_matrix() const override;

    inline virtual SparseAdjacencyMatrix sparse_adjacency_matrix() const override;

    inline virtual std::size_t order() const override;

    inline virtual std::size_t size() const override;

    inline virtual VIDs vertices() const override;

    inline virtual bool has_vertex(const VID &X) const override;

    inline virtual VID add_vertex() override;

    inline virtual void add_vertex(const VID &X) override;

    inline virtual void del_vertex(const VID &X) override;

    inline virtual EIDs edges() const override;

    inline virtual bool has_edge(const EID &X) const override;

    inline virtual void add_edge(const EID &X) override;

    inline virtual void del_edge(const EID &X) override;

   private:
    inline virtual std::size_t hash() const override;

    virtual void print(std::ostream &out) const override;
};
