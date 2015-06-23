[![Build Status](https://travis-ci.org/pbjd/pbdagcon.svg?branch=master)](https://travis-ci.org/pbjd/pbdagcon)
What is pbdagcon?
=================

pbdagcon is a tool that implements DAGCon (Directed Acyclic Graph Consensus)
which is a sequence consensus algorithm based on using directed acyclic graphs
to encode multiple sequence alignment.

It uses the alignment information from blasr to align sequence reads to a
"backbone" sequence. Based on the underlying alignment directed acyclic graph
(DAG), it will be able to use the new information from the reads to find the
discrepancies between the reads and the "backbone" sequences.  A dynamic
programming process is then applied to the DAG to find the optimum sequence of
bases as the consensus.  The new consensus can be used as a new backbone
sequence to iteratively improve the consensus quality.

While the code is developed for processing PacBio(TM) raw sequence data, the
algorithm can be used for general consensus purpose. Currently, it only takes
FASTA input. For shorter read sequences, one might need to adjust the blasr
alignment parameters to get the alignment string properly.

The code and the underlying graphical data structure have been used for some
algorithm development prototyping including phasing reads, pre-assembly and a
work around to generate consensus from intermediate Celera Assembler outputs.

The initial graphical algorithm was a pure python implementation. Cython was
then use to speed it up.

Check out the example/ directory to see how to use it. 

This code is released under the assumption it will help the community to adopt
the PacBio data and make interesting science project possible and more
feasible.  It is not an official software release from the PacBio(TM) software
developing organization.

Building
========
The following are instructions on how to build the C++ pbdagcon executable. 
The code now depends on C++11 features, in particular std::thread, std::move. 
GCC 4.8.1 or higher is known to work.

This project requires that you have boost headers available.  You can either 
supply them yourself or the Makefile will obtain them for you from the 
internet.

### Compile/Check (pbdagcon)
```sh
    # First fetch and build the relevant portions of the blasr_libcpp
    # submodule
    make init-submodule

    # build pbdagcon executable (Makefile fetches boost headers)
    make
    # You already have boost headers
    make boost=<path to headers>

    # build and run unit tests
    make check

    # usage 
    cd src/cpp
    ./pbdagcon --help
```

Running
=======

### Use Case: Generating consensus from BLASR alignments
The most basic use case where one can generate a consensus from a set of 
alignments using the pbdagcon executable directly.

At the most basic level, pbdagcon takes information from BLASR alignments 
sorted by target and generates fasta-formatted corrected target sequences.
The alignments from BLASR can be formatted with either *-m 4* or *-m 5*. 
For *-m 4* format, the alignments must be run through a format adapter, 
*[m4topre.py][]*, in order to generate suitable input to *pbdagcon*.

The following example shows the simplest way to generate a consensus for one 
target using BLASR *-m 5* alignments as input.

    blasr queries.fasta target.fasta -bestn 1 -m 5 -out mapped.m5
    pbdagcon mapped.m5 > consensus.fasta

### Use Case: Generating consensus from daligner alignments
Can parse LAS/DB files generated from the following commits:
* [DALIGNER](https://github.com/thegenemyers/DALIGNER/commit/8edd180ba7b5302c6f1fc859eef5c646db99fd87)
* [DAZ_DB](https://github.com/thegenemyers/DAZZ_DB/commit/84fa98fde94ba0ab56dd715aa7f8fe7e150290f8)


### Use Case: HGAP correction of PacBio reads
Walks through how one could use pbdagcon to correct PacBio reads.  This 
example demonstrates how correction is performed in PacBio's "Hierarchichal  
Genome Assembly Process" (HGAP) workflow.  HGAP uses BLASR *-m 4* output.

This example makes use of the *[filterm4.py][]* and *[m4topre.py][]* scripts:
```sh
    # First filter the m4 file to help remove chimeras
    filterm4.py mapped.m4 > mapped.m4.filt

    # Next run the m4 adapter script, generating 'pre-alignments'
    m4topre.py mapped.m4.filt mapped.m4.filt reads.fasta 24 > mapped.pre

    # Finally, correct using pbdagcon, typically using multiple consensus  
    # threads.
    pbdagcon -j 4 -a mapped.pre > corrected.fasta
```

The *[pbdagcon_wf.sh][]* script automates this workflow.

-----------------------------------------------------------------------------

<script>
(function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){
(i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),
m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)
})(window,document,'script','//www.google-analytics.com/analytics.js','ga');
ga('create', 'UA-13166584-17', 'github.com');
ga('send', 'pageview');
</script>

[m4topre.py]:
  src/m4topre.py 'code'
[filterm4.py]:
  src/filterm4.py 'code'
[pbdagcon_wf.sh]:
  src/cpp/pbdagcon_wf.sh 'code'
