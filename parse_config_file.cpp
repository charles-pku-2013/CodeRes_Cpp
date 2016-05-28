 
void parse_config_file( const char *filename, PropertyTable &propTable )
{
    ifstream ifs(filename, ios::in);

    if (!ifs)
        THROW_RUNTIME_ERROR("parse_config_file() cannot open "
                << filename << " for reading!");

    propTable.clear();

    string line, key, value;
    uint32_t lineno = 0;

    while (getline(ifs, line)) {
        ++lineno;

        string::size_type posComment = line.find_first_of( '#' );
        if (posComment != string::npos)
            line.resize( posComment );
        strip_string( line );
        if (line.empty())
            continue;

        string::size_type posAssign = line.find_first_of( '=' );
        if (posAssign == string::npos)
            throw InvalidInput( stringstream() << "In " << filename << " line " << lineno
                   << " no \'=\' found!" );
        string left = line.substr(0, posAssign);
        string right = line.substr( posAssign + 1 );
        if (right.empty())
            throw InvalidInput( stringstream() << "In " << filename << " line " << lineno
                   << " value field cannot be empty!" );
        key = strip_string( left );
        if (key.empty())
            throw InvalidInput( stringstream() << "In " << filename << " line " << lineno
                   << " invalid property name!" );
        // read values seperated by ':'
        stringstream inStream(right);
        while (getline(inStream, value, ';')) {
            strip_string( value );
            if (value.empty())
                throw InvalidInput( stringstream() << "In " << filename << " line " << lineno
                        << " invalid property value!" );
            propTable[key].insert( value );
        } // while
    } // while

    return;
}
