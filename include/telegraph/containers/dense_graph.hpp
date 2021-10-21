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

    //! Default constructor for a new Dense Graph object.
    DenseGraph();

    //! Copy constructor for a new Dense Graph object.
    DenseGraph(const DenseGraph &other);

    //! Assignment constructor for a new Dense Graph object.
    DenseGraph &operator=(const DenseGraph &other);

    //! Destroy the Dense Graph object.
    virtual ~DenseGraph();

    /**
     * @brief Construct a new Dense Graph object.
     *
     * @param n Given order of the new Dense Graph object.
     */
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

    /**
     * @brief Construct a new Dense Graph object from Adjacency List.
     *
     * @param other Given Adjacency List.
     */
    explicit DenseGraph(const AdjacencyList &other);

    /**
     * @brief Construct a new Dense Graph object from Adjacency Matrix.
     *
     * @param other Given Adjacency Matrix.
     */
    explicit DenseGraph(const AdjacencyMatrix &other);

    /**
     * @brief Construct a new Dense Graph object from Sparse Adjacency Matrix.
     *
     * @param other Given Sparse Adjacency Matrix.
     */
    explicit DenseGraph(const SparseAdjacencyMatrix &other);

    inline virtual operator AdjacencyList() const;

    inline virtual operator AdjacencyMatrix() const;

    inline virtual operator SparseAdjacencyMatrix() const;

    //! VIDs Iterator proxy class.
    class VIDsIterator {
       private:
        //! Pointer to the target graph.
        const DenseGraph *G;

        // Construct a new VIDsIterator object from target graph.
        VIDsIterator(const DenseGraph *G);

       public:
        friend DenseGraph;

        //! Default constructor for a new VIDs Iterator object.
        VIDsIterator();

        //! Copy constructor for a new VIDs Iterator object.
        VIDsIterator(const VIDsIterator &other);

        //! Assignment constructor for a new VIDs Iterator object.
        VIDsIterator &operator=(const VIDsIterator &other);

        //! Destroy the VIDs Iterator object.
        ~VIDsIterator();

        //! VIDs Const Iterator.
        class const_iterator {
           private:
            //! Pointer to the target graph.
            const DenseGraph *G;
            //! Value of the current VID.
            VID curr;

            // Construct a new VIDs Const Iterator object from target graph and initial VID.
            const_iterator(const DenseGraph *G, const VID &curr);

           public:
            friend VIDsIterator;

            using difference_type = std::ptrdiff_t;
            using value_type = VID;
            using pointer = const VID *;
            using reference = const VID &;
            using iterator_category = std::bidirectional_iterator_tag;

            //! Default constructor for a new VIDs Const Iterator object.
            const_iterator();

            //! Copy constructor for a new VIDs Const Iterator object.
            const_iterator(const const_iterator &other);

            //! Assignment constructor for a new VIDs Const Iterator object.
            const_iterator &operator=(const const_iterator &other);

            //! Destroy the VIDs Const Iterator object.
            ~const_iterator();

            //! Equality operator.
            inline bool operator==(const const_iterator &other) const;

            //! Inequality operator.
            inline bool operator!=(const const_iterator &other) const;

            //! Dereference operator.
            inline reference operator*() const;

            //! Member access operator.
            inline reference operator->() const;

            //! Pre-increment operator.
            inline const_iterator &operator++();

            //! Post-increment operator.
            inline const_iterator operator++(int);

            //! Pre-decrement operator.
            inline const_iterator &operator--();

            //! Post-decrement operator.
            inline const_iterator operator--(int);
        };

        //! Begin iterator.
        const_iterator begin() const;

        //! End iterator.
        const_iterator end() const;
    };

    /**
     * @brief Return the VIDs Iterator proxy class.
     *
     * @return VIDsIterator VIDs Iterator proxy class.
     */
    VIDsIterator V() const;

        //! VIDs Iterator proxy class.
    class VLBsIterator {
       private:
        //! Pointer to the target graph.
        const DenseGraph *G;

        // Construct a new VLBsIterator object from target graph.
        VLBsIterator(const DenseGraph *G);

       public:
        friend DenseGraph;

        //! Default constructor for a new VLBs Iterator object.
        VLBsIterator();

        //! Copy constructor for a new VLBs Iterator object.
        VLBsIterator(const VLBsIterator &other);

        //! Assignment constructor for a new VLBs Iterator object.
        VLBsIterator &operator=(const VLBsIterator &other);

        //! Destroy the VLBs Iterator object.
        ~VLBsIterator();

        //! VLBs Const Iterator.
        class const_iterator {
           private:
            //! Pointer to the target graph.
            const DenseGraph *G;
            //! Value of the current VID.
            VID curr;

            // Construct a new VLBs Const Iterator object from target graph and initial VLB.
            const_iterator(const DenseGraph *G, const VID &curr);

           public:
            friend VLBsIterator;

            using difference_type = std::ptrdiff_t;
            using value_type = VLB;
            using pointer = const VLB *;
            using reference = const VLB &;
            using iterator_category = std::bidirectional_iterator_tag;

            //! Default constructor for a new VLBs Const Iterator object.
            const_iterator();

            //! Copy constructor for a new VLBs Const Iterator object.
            const_iterator(const const_iterator &other);

            //! Assignment constructor for a new VLBs Const Iterator object.
            const_iterator &operator=(const const_iterator &other);

            //! Destroy the VLBs Const Iterator object.
            ~const_iterator();

            //! Equality operator.
            inline bool operator==(const const_iterator &other) const;

            //! Inequality operator.
            inline bool operator!=(const const_iterator &other) const;

            //! Dereference operator.
            inline reference operator*() const;

            //! Member access operator.
            inline reference operator->() const;

            //! Pre-increment operator.
            inline const_iterator &operator++();

            //! Post-increment operator.
            inline const_iterator operator++(int);

            //! Pre-decrement operator.
            inline const_iterator &operator--();

            //! Post-decrement operator.
            inline const_iterator operator--(int);
        };

        //! Begin iterator.
        const_iterator begin() const;

        //! End iterator.
        const_iterator end() const;
    };

    /**
     * @brief Return the VLBs Iterator proxy class.
     *
     * @return VLBsIterator VLBs Iterator proxy class.
     */
    VLBsIterator Vl() const;

    /*
    class EIDsIterator;

    class ELBsIterator;
    */

    inline virtual std::size_t order() const;

    inline virtual std::size_t size() const;

    inline virtual bool has_vertex(const VID &X) const;

    inline virtual VID add_vertex();

    inline virtual VID add_vertex(const VID &X);

    inline virtual VID del_vertex(const VID &X);

    inline virtual bool has_edge(const EID &X) const;

    inline virtual EID add_edge(const EID &X);

    inline virtual EID del_edge(const EID &X);

   private:
    inline virtual std::size_t hash() const;

    virtual void to_stream(std::ostream &out) const;
};
