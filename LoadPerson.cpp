struct ConnectorParams {
    ConnectorParams& SetDbAllowExceptions(bool db_allow_exceptions) {
        this->db_allow_exceptions_ = db_allow_exceptions;
        return *this;
    }
    ConnectorParams& SetDbLogLevel(DBLogLevel db_log_level) {
        this->db_log_level_ = db_log_level;
        return *this;
    }
    bool db_allow_exceptions_;
    DBLogLevel db_log_level_;
};

struct DbParams {
    DbParams& SetDbName(string_view db_name) {
        this->db_name_ = db_name;
        return *this;
    }
    DbParams& SetDbConnectionTimeout(int db_connection_timeout) {
        this->db_connection_timeout_ = db_connection_timeout;
        return *this;
    }
    string_view db_name_;
    int db_connection_timeout_;
};


struct DBQueryParams {
    DBQueryParams& SetMaxAge(int max_age) {
        this->max_age_ = max_age;
        return *this;
    }
    DBQueryParams& SetMinAge(int min_age) {
        this->min_age_ = min_age;
        return *this;
    }
    DBQueryParams& SetNameFilter(string_view name_filter) {
        this->name_filter_ = name_filter;
        return *this;
    }

    void FormQuery(ostream& query_str, DBHandler& db) {
        query_str << "from Persons "s
            << "select Name, Age "s
            << "where Age between "s << min_age_ << " and "s << max_age_ << " "s
            << "and Name like '%"s << db.Quote(name_filter_) << "%'"s;
    }
    int max_age_;
    int min_age_;
    string_view name_filter_;
};

vector<Person> LoadPersons(ConnectorParams connector_params, DbParams db_params, DBQueryParams db_query) {
    DBConnector connector(connector_params);
    DBHandler db;
    if (db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_params);
    }
    else {
        db = connector.Connect(db_params);
    }
    if (!db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    db_query.FormQuery(query_str, db);
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({ move(name), age });
    }
    return persons;
}