#pragma once

class DirectedGraph {
   protected:
    //! Default constructor for a new Directed Graph object.
    DirectedGraph();

   public:
    /**
     * @brief Whether the graph is defined as directed or not.
     *
     * @return true If the graph is defined as directed,
     * @return false Otherwise.
     */
    constexpr static inline bool is_directed();

    /**
     * @brief Whether the graph is defined as partially directed or not.
     *
     * @return true If the graph is defined as partially directed,
     * @return false Otherwise.
     */
    constexpr static inline bool is_partially_directed();
};
