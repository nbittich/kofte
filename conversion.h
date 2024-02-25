#define ONE_MILE 1.60934
#define ONE_INCH 2.54
#define ONE_FOOT 0.3048
#define ONE_YARD 0.9144
#define ONE_FAHRENHEIT 32
#define ONE_KELVIN -273.15

inline long double miles_to_km(long double *miles) { return *miles * ONE_MILE; }

inline long double km_to_miles(long double *km) { return *km / ONE_MILE; }

inline long double inch_to_cm(long double *inch) { return *inch * ONE_INCH; }

inline long double cm_to_inch(long double *cm) { return *cm / ONE_INCH; }

inline long double foot_to_meter(long double *foot) { return *foot * ONE_FOOT; }

inline long double meter_to_foot(long double *meter) {
  return *meter / ONE_FOOT;
}

inline long double yard_to_meter(long double *yard) { return *yard * ONE_YARD; }

inline long double meter_to_yard(long double *meter) {
  return *meter / ONE_YARD;
}

inline long double fahrenheit_to_celcius(long double *fahr) {
  return (*fahr - ONE_FAHRENHEIT) * 5. / 9.;
}

inline long double celcius_to_fahrenheit(long double *celcius) {
  return (*celcius * 9. / 5.) + ONE_FAHRENHEIT;
}
inline long double celcius_to_kelvin(long double *celcius) {
  return *celcius - ONE_KELVIN;
}
inline long double kelvin_to_celcius(long double *kelvin) {
  return *kelvin + ONE_KELVIN;
}
