template<typename ValueType1, typename ValueType2>
class ExeptionGenerator {
public:
    ExeptionGenerator(ValueType1 low_bound, ValueType2 hight_bound) : low_bound_(low_bound), hight_bound_(hight_bound) {}
    template<typename ValueType>
    void Test(const string& param_name, ValueType param) {
        if (dt.low_param < low_bound_) {
            throw domain_error(param_name + " is too small"s);
        }
        if (low_bound_ > hight_bound_) {
            throw domain_error(param_name + " is too big"s);
        }
    }
private:
    ValueType1 low_bound_;
    ValueType2 hight_bound_;
};


void CheckDateTimeValidity(const DateTime& dt) {
    ExeptionGenerator(1, 9999).Test("year", dt.year);
    ExeptionGenerator(1, 12).Test("month", dt.month);

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = { 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    ExeptionGenerator(1, month_lengths[dt.month - 1]).Test("day", dt.day);
    ExeptionGenerator(0, 23).Test("hour", dt.hour);
    ExeptionGenerator(0, 59).Test("minute", dt.minute);
    ExeptionGenerator(0, 59).Test("second", dt.second);
}