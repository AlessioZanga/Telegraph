#pragma once

#include <telegraph/containers/abstract_graph.ipp>

class DenseGraph : public AbstractGraph {
   protected:
    AdjacencyMatrix A;

   public:
    //! Enforce superclass namespace resolution to avoid name hiding with overloading.
    using AbstractGraph::add_edge;
    using AbstractGraph::add_vertex;
    using AbstractGraph::del_edge;
    using AbstractGraph::del_vertex;
    using AbstractGraph::has_edge;
    using AbstractGraph::has_vertex;

    DenseGraph();

    DenseGraph(const DenseGraph &other);

    DenseGraph &operator=(const DenseGraph &other);

    virtual ~DenseGraph();

    explicit DenseGraph(std::size_t n);

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
    template <typename I, require_iter_value_type(I, VID) = 0>
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
    template <typename I, require_iter_value_type(I, EID) = 0>
    DenseGraph(const I &begin, const I &end);

    explicit DenseGraph(const AdjacencyList &other);

    explicit DenseGraph(const AdjacencyMatrix &other);

    explicit DenseGraph(const SparseAdjacencyMatrix &other);

    inline virtual operator AdjacencyList() const;

    inline virtual operator AdjacencyMatrix() const;

    inline virtual operator SparseAdjacencyMatrix() const;

    inline virtual std::size_t order() const;

    inline virtual std::size_t size() const;

    inline virtual bool has_vertex(const VID &X) const;

    inline virtual VID add_vertex();

    inline virtual VID add_vertex(const VID &X);

    inline virtual VID del_vertex(const VID &X);

    inline virtual bool has_edge(const EID &X) const;

    inline virtual EID add_edge(const EID &X);

    inline virtual EID del_edge(const EID &X);

    class VIDsIterator {
       private:
        std::size_t order;

        VIDsIterator(const std::size_t &order);

       public:
        friend DenseGraph;

        VIDsIterator();
        VIDsIterator(const VIDsIterator &other);
        VIDsIterator &operator=(const VIDsIterator &other);
        ~VIDsIterator();

        class const_iterator {
           private:
            std::size_t order;
            VID current;

            const_iterator(const std::size_t &order, const VID &current);

           public:
            friend VIDsIterator;

            using difference_type = std::ptrdiff_t;
            using value_type = VID;
            using pointer = const VID *;
            using reference = const VID &;
            using iterator_category = std::bidirectional_iterator_tag;

            const_iterator();
            const_iterator(const const_iterator &other);
            const_iterator &operator=(const const_iterator &other);
            ~const_iterator();

            inline bool operator==(const const_iterator &other) const;
            inline bool operator!=(const const_iterator &other) const;

            inline reference operator*() const;
            inline reference operator->() const;

            inline const_iterator &operator++();
            inline const_iterator operator++(int);

            inline const_iterator &operator--();
            inline const_iterator operator--(int);
        };

        const_iterator begin() const;
        const_iterator end() const;
    };

    VIDsIterator V() const;

    /*
    class VLBsIterator;

    class EIDsIterator;

    class ELBsIterator;
    */

   private:
    inline virtual std::size_t hash() const;

    virtual void to_stream(std::ostream &out) const;
};
