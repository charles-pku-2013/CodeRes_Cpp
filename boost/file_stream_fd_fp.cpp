
auto run_vocab_count = [&] {
    typedef boost::iostreams::stream< boost::iostreams::file_descriptor_source >
        FDStream;

    string vocabCmd, line;

    stringstream str;
    str << "./vocab_count.bin -verbose 0 ";
    str << "-min-count " << g_nMinCount;
    if (g_nMaxVocab)
        str << " -max-vocab " << g_nMaxVocab;
    str << " < " << g_cstrInputData << flush;
    vocabCmd = std::move(str.str());

    // LOG(INFO) << "vocabCmd = " << vocabCmd;
    ofstream vocabOutFile( vocabOutFilename, ios::out );
    if (!vocabOutFile)
        throw_runtime_error("Cannot open vocabOutFile for writting!");

    FILE *fp = ::popen(vocabCmd.c_str(), "r");
    if (!fp)
        throw_runtime_error("Launching vocab_count failed!");

    setlinebuf(fp);

    FDStream pipeStream( fileno(fp), boost::iostreams::never_close_handle );

    StringPtr    pWord;
    uint32_t     count;
    while (getline(pipeStream, line)) {
        // NOTE!! output on screen is stdout and stderr mixed
        // cerr << "line: " << line << endl;
        pWord.reset( new string );
        stringstream str(line);
        str >> *pWord >> count;
        g_pFreqDB->addItem( pWord, count );
        vocabOutFile << line << endl;
    } // while
    ::pclose(fp);
};

