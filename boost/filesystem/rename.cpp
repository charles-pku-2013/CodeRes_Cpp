
try {
    boost::filesystem::rename(oldName, newName);
} catch (const boost::filesystem::filesystem_error &ex) {
    cerr << "Renaming fail: " << ex.what() << endl;
} // try
