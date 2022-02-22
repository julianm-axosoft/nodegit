var NodeGit = require("../");

var Libgit2 = NodeGit.Libgit2;

Libgit2.OPT.SET_WINDOWS_LONGPATHS = 31;
Libgit2.OPT.GET_WINDOWS_LONGPATHS = 32;
Libgit2.OPT.SET_CHECKOUT_THREADS = 33;
Libgit2.OPT.GET_CHECKOUT_THREADS = 34;
