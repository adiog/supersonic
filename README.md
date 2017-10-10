# Supersonic [![Build Status](https://travis-ci.org/adiog/supersonic.svg?branch=modern)](https://travis-ci.org/adiog/supersonic)

# Copyright 2012 Google Inc. All Rights Reserved.
Original installation instructions are provided in INSTALL.
Currently project can be built with cmake.

Introducing Supersonic Query Engine
===================================

Monday, October 15, 2012

We are happy to announce the open source release of Supersonic, a query engine library, that is extremely useful for creating a column oriented database back-end.

Supersonic’s main strength lies in its speed. It is a cache-aware engine which exploits several low-level optimization techniques to ensure second-to-none execution times and high throughput. By making use of SIMD instructions and efficient pipelining we make columnar data processing very fast.

The engine supports a wide variety of typical columnar operations as well as many other specialized functionalities. We believe it to be a useful tool for those of you working on new DBMS solutions to help handle your data more effectively.

![](https://github.com/adiog/supersonic/raw/modern/docs/query_plan.png "")

For some pointers on how to get started with the library visit the project page on Google Code. We also host a **[discussion group](https://groups.google.com/forum/?fromgroups#!forum/supersonic-query-engine)** where you can get up to speed on the latest development news.

Intro
=====

Check the **[API presentation](https://github.com/adiog/supersonic/raw/modern/docs/api-presentation.pdf)**
