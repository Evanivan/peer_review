
struct Db_params {
    string_view db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
};

struct Person_params {
    int min_age;
    int max_age;
    string_view name_filter;
};

static const string intro = "from Persons"
                " select Name, Age "
                "where Age between "s;

ostringstream& InfoMessage(ostringstream& ostr, const DBHandler& db, const Person_params& person_p)
{
    ostr << intro << person_p.min_age << " and "s << person_p.max_age << " "
              << "and Name like '%"s << db.Quote(person_p.name_filter) << "%'"s;
    return ostr;
}

vector<Person> LoadPersons(Db_params db_p, Person_params person_p) {
    DBConnector connector(db_p.db_allow_exceptions, db_p.db_log_level);
    DBHandler db;
    if (db_p.db_name.starts_with("tmp."s)) {
        db = connector.ConnectTmp(db_p.db_name, db_p.db_connection_timeout);
    } else {
        db = connector.Connect(db_p.db_name, db_p.db_connection_timeout);
    }
    if (!db_p.db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;

    DBQuery query(InfoMessage(query_str, db, person_p).str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}