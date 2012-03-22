///----------------------------------------------
// Copyright 2012 Wellcome Trust Sanger Institute
// Written by Jared Simpson (js18@sanger.ac.uk)
// Released under the GPL
//-----------------------------------------------
//
// ReadCoherentHaplotypeBuilder - Build read coherent haplotypes
// using walks through a de Bruijn graph.
//
#ifndef READ_COHERENT_HAPLOTYPE_BUILDER_H
#define READ_COHERENT_HAPLOTYPE_BUILDER_H
#include "BWT.h"
#include "SampledSuffixArray.h"
#include "BWTInterval.h"
#include "BWTIntervalCache.h"
#include "SGUtil.h"
#include "SGWalk.h"
#include "VariationBubbleBuilder.h"
#include "HaplotypeBuilder.h"
#include "multiple_alignment.h"
#include "GraphCompare.h"
#include <queue>

// Structure holding a read and its inferred position on the haplotype
struct HaplotypePositionedRead
{
    // functions
    friend bool operator<(const HaplotypePositionedRead& a, const HaplotypePositionedRead& b) { return a.position < b.position; }
    static bool sortByID(const HaplotypePositionedRead& a, const HaplotypePositionedRead& b) { return a.id < b.id; }
    static bool equalByID(const HaplotypePositionedRead& a, const HaplotypePositionedRead& b) { return a.id == b.id; }

    // data
    std::string id;
    std::string sequence;
    int position; // relative to the initial kmer, which is position 0.
};

typedef std::vector<HaplotypePositionedRead> HaplotypeReadVector;

// Build haplotypes starting from a given sequence.
class ReadCoherentHaplotypeBuilder
{
    public:

        ReadCoherentHaplotypeBuilder();
        ~ReadCoherentHaplotypeBuilder();

        void setInitialHaplotype(const std::string& str);
        void setParameters(const GraphCompareParameters& parameters); 

        // Run the bubble construction process
        // Returns true if the graph was successfully built between the two sequences
        HaplotypeBuilderReturnCode run(StringVector& out_haplotypes);
        
    private:
    
        // Find reads with the given kmer and calculate their inferred position on the haplotype we are building
        void addPositionedReadsForKmers(const std::string& consensus, const std::vector<std::string>& kmer_vector, HaplotypeReadVector* positioned_reads);

        // Build a multiple alignment of all the reads that are potentially part of this haplotype
        MultipleAlignment buildMultipleAlignment(HaplotypeReadVector& positioned_reads) const;

        // Compute a consensus sequence for the multiple alignment
        std::string getConsensus(MultipleAlignment* multiple_alignment, int min_call_coverage, int max_differences) const;

        // Check whether the input sequence has any unique kmers that can be used to extend the haplotype
        std::vector<std::string> getExtensionKmers(const std::string& sequence);

        //
        // Data
        //
        GraphCompareParameters m_parameters;
        std::string m_initial_kmer_string;
        std::set<std::string> m_used_kmers;
};

#endif