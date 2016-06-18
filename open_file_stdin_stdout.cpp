boost::shared_ptr<std::istream> open_input()
{
    auto deleteIfs = [](std::istream *p) {
        // DLOG_IF(INFO, p == &cin) << "Not delete cin";
        // DLOG_IF(INFO, p && p != &cin) << "Deleting ordinary in file obj";
        if (p && p != &cin)
            delete p;
    };

    boost::shared_ptr<std::istream> ifs(nullptr, deleteIfs);
    if (FLAGS_input == "-") {
        ifs.reset( &cin, deleteIfs );
    } else {
        ifs.reset( new ifstream(FLAGS_input, ios::in), deleteIfs );
        if ( !ifs || !*ifs)
            THROW_RUNTIME_ERROR("Cannot open " << FLAGS_input << " for reading!");
    } // if

    return ifs;
}

boost::shared_ptr<std::ostream> open_output()
{
    auto deleteOfs = [](std::ostream *p) {
        // DLOG_IF(INFO, p == &cout) << "Not delete cout";
        // DLOG_IF(INFO, p && p != &cout) << "Deleting ordinary out file obj";
        if (p && p != &cout)
            delete p;
    };

    boost::shared_ptr<std::ostream> ofs(nullptr, deleteOfs);
    if (FLAGS_output == "-") {
        ofs.reset( &cout, deleteOfs );
    } else {
        ofs.reset( new ofstream(FLAGS_output, ios::out), deleteOfs );
        if ( !ofs || !*ofs)
            THROW_RUNTIME_ERROR("Cannot open " << FLAGS_output << " for writting!");
    } // if

    return ofs;
}

