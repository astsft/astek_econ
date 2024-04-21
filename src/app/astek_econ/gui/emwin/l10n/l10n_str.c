/**
  * @file    l10n_str.c
  * @brief   User Interface - Display - Localization
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdlib.h>
#include "l10n.h"


const l10n_str_t l10n_list_language[]          = { "ENGLISH", "РУССКИЙ", };
const l10n_str_t l10n_list_relay_mode_en[]     = { "THRESHOLD", "ALARM", "NOT ACTIVE"};
const l10n_str_t l10n_list_relay_mode_ru[]     = { "ПОРОГ", "ТРЕВОГА", "НЕ АКТИВНА"};

const l10n_str_t l10n_str_arrow_leftwards[]             = { "⮈",                                        "⮈",                              }; 
const l10n_str_t l10n_str_arrow_upwards[]               = { "⮉",                                        "⮉",                              }; 
const l10n_str_t l10n_str_arrow_rightwards[]            = { "⮊",                                        "⮊",                              }; 
const l10n_str_t l10n_str_arrow_downwards[]             = { "⮋",                                        "⮋",                              }; 
const l10n_str_t l10n_str_allowed[]                     = { "ALLOWED",                                  "РАЗРЕШЕНО",                      };
const l10n_str_t l10n_str_auto[]                        = { "AUTO",                                     "АВТО",                           };
const l10n_str_t l10n_str_auto_calibration[]            = { "AUTO CALIBRATION",                         "АВТОКАЛИБРОВКА",                 };
const l10n_str_t l10n_str_autolaunch[]                  = { "AUTOLAUNCH",                               "АВТОЗАПУСК",                     };
const l10n_str_t l10n_str_back[]                        = { "✘  BACK",                                  "✘  НАЗАД",                       };
const l10n_str_t l10n_str_cancel[]                      = { "✘  CANCEL",                                "✘  ОТМЕНА",                      };
const l10n_str_t l10n_str_calibrate[]                   = { "CALIBRATE",                                "КАЛИБРОВАТЬ",                    };
const l10n_str_t l10n_str_calibrate_al[]                = { "⮈  CALIBRATE",                             "⮈  КАЛИБРОВКА",                  };
const l10n_str_t l10n_str_calibrate_ar[]                = { "CALIBRATE  ⮊",                             "КАЛИБРОВКА  ⮊",                  };
const l10n_str_t l10n_str_calibration[]                 = { "CALIBRATION",                              "КАЛИБРОВКА",                     };
const l10n_str_t l10n_str_calibration_channel1[]        = { "CHANNEL 1",                                "КАНАЛ 1",                        };
const l10n_str_t l10n_str_calibration_channel2[]        = { "CHANNEL 2",                                "КАНАЛ 2",                        };
const l10n_str_t l10n_str_calibration_correction[]      = { "CALIBRATION CORRECTION",                   "КОРРЕКЦИЯ КАЛИБРОВКИ"            };
const l10n_str_t l10n_str_cl420[]                       = { "4-20 mA",                                  "4-20 mA",                        };
const l10n_str_t l10n_str_cloop[]                       = { "CURRENT LOOP",                             "ТОКОВАЯ ПЕТЛЯ",                  };
const l10n_str_t l10n_str_cloop_error[]                 = { "ERROR CURRENT",                            "ТОК ОШИБКИ",                     };
const l10n_str_t l10n_str_cloop_4mA[]                   = { " 4 mA",                                    " 4 mA",                          };
const l10n_str_t l10n_str_cloop_20mA[]                  = { " 20 mA",                                   " 20 mA",                         };
const l10n_str_t l10n_str_cloop_3mA[]                   = { " 3 mA",                                    " 3 mA",                          };
const l10n_str_t l10n_str_cloop_22mA[]                  = { " 22 mA",                                   " 22 mA",                         };
const l10n_str_t l10n_str_coarse[]                      = { "COARSE ADJUSTMENT",                        "ГРУБАЯ ПОДСТРОЙКА",              };
const l10n_str_t l10n_str_coefficient[]                 = { "COEFFICIENT =",                            "КОЭФФИЦИЕНТ =",                  };
const l10n_str_t l10n_str_completed[]                   = { "COMPLETED",                                "ВЫПОЛНЕНО",                      };
const l10n_str_t l10n_str_confirm[]                     = { "CONFIRM",                                  "ПОДТВЕРДИТЬ",                    };
const l10n_str_t l10n_str_continuous[]                  = { "CONTINUOUS",                               "НЕПРЕРЫВНО",                     };
const l10n_str_t l10n_str_console[]                     = { "JOURNAL",                                  "ЖУРНАЛ",                         };
const l10n_str_t l10n_str_console_al[]                  = { "⮈  JOURNAL",                               "⮈  ЖУРНАЛ",                      };
const l10n_str_t l10n_str_console_ar[]                  = { "JOURNAL  ⮊",                               "ЖУРНАЛ  ⮊",                      };
const l10n_str_t l10n_str_correction[]                  = { "CORRECTION",                               "КОРРЕКЦИЯ",                      };
const l10n_str_t l10n_str_cycle[]                       = { "CYCLE",                                    "ЦИКЛ",                           };
const l10n_str_t l10n_str_cycle_csm[]                   = { "CSM CYCLE",                                "CSM ЦИКЛ",                       };
const l10n_str_t l10n_str_cycle_czm[]                   = { "CZM CYCLE",                                "CZM ЦИКЛ",                       };
const l10n_str_t l10n_str_cycle_pcsm[]                  = { "PCSM CYCLE",                               "PCSM ЦИКЛ",                      };
const l10n_str_t l10n_str_cycle_pczm[]                  = { "PCZM CYCLE",                               "PCZM ЦИКЛ",                      };
const l10n_str_t l10n_str_date[]                        = { "DATE",                                     "ДАТА",                           };
const l10n_str_t l10n_str_day[]                         = { "DAY",                                      "ДЕНЬ",                           };
const l10n_str_t l10n_str_days[]                        = { "DAYS",                                     "ДНИ",                            };
const l10n_str_t l10n_str_debug[]                       = { "DEBUG",                                    "ОТЛАДКА",                        };
const l10n_str_t l10n_str_diagnostics[]                 = { "DIAGNOSTICS",                              "ДИАГНОСТИКА",                    };
const l10n_str_t l10n_str_disabled[]                    = { "DISABLED",                                 "ОТКЛЮЧЕНО",                      };
const l10n_str_t l10n_str_discrete_output[]             = { "DISCRETE OUTPUT",                          "ДИСКРЕТНЫЙ ВЫХОД",               };
const l10n_str_t l10n_str_enabled[]                     = { "ENABLED",                                  "ВКЛЮЧЕНО",                       };
const l10n_str_t l10n_str_english[]                     = { "ENGLISH",                                  "ENGLISH",                        };
const l10n_str_t l10n_str_enter[]                       = { "✔  ENTER",                                 "✔  ВВОД",                       };
const l10n_str_t l10n_str_error[]                       = { "ERROR",                                    "ОШИБКА",                         };
const l10n_str_t l10n_str_factory_settings[]            = { "FACTORY SETTINGS",                         "ЗАВОДСКИЕ НАСТРОЙКИ",             };
const l10n_str_t l10n_str_filter[]                      = { "FILTER",                                   "ФИЛЬТР",                         };
const l10n_str_t l10n_str_filter_cutoff[]               = { "CUTOFF",                                   "ЧАСТОТА",                        };
const l10n_str_t l10n_str_filter_order[]                = { "ORDER",                                    "ПОРЯДОК",                        };
const l10n_str_t l10n_str_fine[]                        = { "FINE ADJUSTMENT",                          "ТОЧНАЯ ПОДСТРОЙКА",              };
const l10n_str_t l10n_str_firmware_ver[]                = { "FW VERSION: ",                             "ВЕРСИЯ П.О.: ",                  };
const l10n_str_t l10n_str_help[]                        = { "HELP",                                     "ПОМОЩЬ",                         };
const l10n_str_t l10n_str_hour_meter[]                  = { "OPERATING, HRS: ",                         "В РАБОТЕ, ЧАСОВ: ",              };
const l10n_str_t l10n_str_hour[]                        = { "HOUR",                                     "ЧАС",                            };
const l10n_str_t l10n_str_info[]                        = { "INFO",                                     "ИНФО",                           };
const l10n_str_t l10n_str_interval[]                    = { "INTERVAL",                                 "ИНТЕРВАЛ",                       };
const l10n_str_t l10n_str_language[]                    = { "LANGUAGE",                                 "ЯЗЫК",                           };
const l10n_str_t l10n_str_lower[]                       = { "LOWER",                                    "НИЖНИЙ",                         };
const l10n_str_t l10n_str_launch[]                      = { "LAUNCH",                                   "ЗАПУСК",                         };
const l10n_str_t l10n_str_measure[]                     = { "MEASURE",                                  "ИЗМЕРЕНИЕ",                      };
const l10n_str_t l10n_str_measure_al[]                  = { "⮈  MEASURE",                               "⮈  ИЗМЕРЕНИЕ",                   };
const l10n_str_t l10n_str_measure_ar[]                  = { "MEASURE  ⮊",                               "ИЗМЕРЕНИЕ  ⮊",                   };
const l10n_str_t l10n_str_middle[]                      = { "MIDDLE",                                   "СРЕДНИЙ",                        };
const l10n_str_t l10n_str_min[]                         = { "MIN",                                      "МИН",                            };
const l10n_str_t l10n_str_minutes[]                     = { "MINUTES",                                  "МИНУТ",                          };
const l10n_str_t l10n_str_mode[]                        = { "MODE",                                     "РЕЖИМ",                          };
const l10n_str_t l10n_str_mdb_relay[]                   = { "MODBUS RELAY",                             "MODBUS РЕЛЕ",                    };
const l10n_str_t l10n_str_month[]                       = { "MONTH",                                    "МЕСЯЦ",                          };
const l10n_str_t l10n_str_over_modbus[]                 = { "OVER MODBUS",                              "ЧЕРЕЗ MODBUS",                   };
const l10n_str_t l10n_str_password[]                    = { "PASSWORD",                                 "ПАРОЛЬ",                         };
const l10n_str_t l10n_str_password_reset[]              = { "PASSWORD RESET",                           "СБРОС ПАРОЛЯ",                   };
const l10n_str_t l10n_str_point_low[]                   = { "LOWER POINT",                              "НИЖНЯЯ ТОЧКА",                   };
const l10n_str_t l10n_str_pressure[]                    = { "PRESSURE",                                 "ДАВЛЕНИЕ",                       };
const l10n_str_t l10n_str_process[]                     = { "PROCESS",                                  "ПРОЦЕСС",                        };
const l10n_str_t l10n_str_prohibited[]                  = { "PROHIBITED",                               "ЗАПРЕЩЕНО",                      };
const l10n_str_t l10n_str_purge[]                       = { "PURGE",                                    "ПРОДУВКА",                       };
const l10n_str_t l10n_str_range[]                       = { "RANGE",                                    "ДИАПАЗОН",                       };
const l10n_str_t l10n_str_remotely[]                    = { "REMOTELY",                                 "УДАЛЕННО",                       };
const l10n_str_t l10n_str_relay[]                       = { "RELAY",                                    "РЕЛЕ",                           };
const l10n_str_t l10n_str_relay_1[]                     = { "RELAY THRESHOLD 1",                        "РЕЛЕ ПОРОГ 1 ",                  };
const l10n_str_t l10n_str_relay_2[]                     = { "RELAY THRESHOLD 2",                        "РЕЛЕ ПОРОГ 2",                   };
const l10n_str_t l10n_str_relay_state_no[]              = { " NORMAL OPEN",                             " НОРМАЛЬНО РАЗОМКНУТЫЙT",        };
const l10n_str_t l10n_str_relay_state_nc[]              = { " NORMAL CLOSE",                            " НОРМАЛЬНО ЗАМКНУТЫЙ",           };
const l10n_str_t l10n_str_restore[]                     = { "RESTORE",                                  "ВОССТАНОВИТЬ",                   };
const l10n_str_t l10n_str_runs_counter[]                = { "STARTS COUNTER: ",                         "СЧЕТЧИК ВКЛЮЧЕНИЙ: ",            };
const l10n_str_t l10n_str_russian[]                     = { "РУССКИЙ",                                  "РУССКИЙ",                        };
const l10n_str_t l10n_str_scale[]                       = { "SCALE",                                    "ШКАЛА",                          };
const l10n_str_t l10n_str_sec[]                         = { "SEC",                                      "СЕК",                            };
const l10n_str_t l10n_str_seconds[]                     = { "SECONDS",                                  "СЕКУНД",                         };
const l10n_str_t l10n_str_select[]                      = { "SELECT",                                   "ВЫБОР",                          };
const l10n_str_t l10n_str_sensor[]                      = { "SENSOR",                                   "СЕНСОР",                         };
const l10n_str_t l10n_str_serial_num[]                  = { "SERIAL NO: ",                              "СЕРИЙНЫЙ №: ",                   };
const l10n_str_t l10n_str_service[]                     = { "SERVICE",                                  "СЕРВИС",                         };
const l10n_str_t l10n_str_setup[]                       = { "SETUP",                                    "НАСТРОЙКА",                      };
const l10n_str_t l10n_str_setup_al[]                    = { "⮈  SETUP",                                 "⮈  НАСТРОЙКИ",                   };
const l10n_str_t l10n_str_setup_ar[]                    = { "SETUP  ⮊",                                 "НАСТРОЙКИ  ⮊",                   };
const l10n_str_t l10n_str_slope[]                       = { "SLOPE",                                    "НАКЛОН",                         };
const l10n_str_t l10n_str_smoothing[]                   = { "SMOOTHING",                                "СГЛАЖИВАНИЕ",                    };
const l10n_str_t l10n_str_state[]                       = { "STATE",                                    "СОСТОЯНИЕ",                      };
const l10n_str_t l10n_str_specify[]                     = { "SPECIFY",                                  "УСТАНОВИТЬ",                     };
const l10n_str_t l10n_str_span[]                        = { "SPAN",                                     "SPAN",                           };
const l10n_str_t l10n_str_success[]                     = { "SUCCESS",                                  "УСПЕШНО",                        };
const l10n_str_t l10n_str_system[]                      = { "SYSTEM",                                   "СИСТЕМА",                        };
const l10n_str_t l10n_str_temperature[]                 = { "TEMPERATURE",                              "ТЕМПЕРАТУРА",                    };
const l10n_str_t l10n_str_timings[]                     = { "TIMINGS",                                  "ХРОНОМЕТРАЖ",                    };
const l10n_str_t l10n_str_threshold[]                   = { "THRESHOLD",                                "ПОРОГ",                          };
const l10n_str_t l10n_str_threshold_1[]                 = { "THRESHOLD 1",                              "ПОРОГ 1",                        };
const l10n_str_t l10n_str_threshold_2[]                 = { "THRESHOLD 2",                              "ПОРОГ 2",                        };
const l10n_str_t l10n_str_time[]                        = { "TIME",                                     "ВРЕМЯ",                          };
const l10n_str_t l10n_str_upper[]                       = { "UPPER",                                    "ВЕРХНИЙ",                        };
const l10n_str_t l10n_str_value[]                       = { "VALUE",                                    "ЗНАЧЕНИЕ",                       };
const l10n_str_t l10n_str_year[]                        = { "YEAR",                                     "ГОД",                            };
const l10n_str_t l10n_str_zero[]                        = { "ZERO",                                     "ZERO",                           };
const l10n_str_t l10n_str_key_on[]                      = { "KEY ON",                   		"КЛЮЧ ВКЛ.",                      };
const l10n_str_t l10n_str_key_off[]                     = { "KEY OFF",                  		"КЛЮЧ ВЫКЛ.",                     };
const l10n_str_t l10n_str_system_startup[]              = { "System startup",                           "Запуск системы",                 };  
const l10n_str_t l10n_str_link_error[]                  = { "Link error",                               "Ошибка связи",                   };
const l10n_str_t l10n_str_temp_measure_err[]            = { "Temperature measurement error",            "Ошибка измерения температуры",   };
const l10n_str_t l10n_str_temp_measure_ok[]             = { "Temperature ok",                           "Температура в норме",            };
const l10n_str_t l10n_str_temp_too_low[]                = { "Temperature too low",                      "Слишком низкая температура",     };
const l10n_str_t l10n_str_temp_too_high[]               = { "Temperature too hi",                       "Слишком высокая температура",    };
const l10n_str_t l10n_str_temp_not_in_range_high[]      = { "Temperature is higher by 20 °C",           "Температура выше на 20 °C",      };
const l10n_str_t l10n_str_temp_not_in_range_low[]       = { "Temperature is lower by 20 °C",            "Температура ниже на 20 °C",      };
const l10n_str_t l10n_str_value_too_low[]               = { "Value too low",                            "Слишком низкое значение",        };
const l10n_str_t l10n_str_value_too_high[]              = { "Value too low",                            "Слишком высокое значение",       };
const l10n_str_t l10n_str_value_ok[]                    = { "Value ok",                                 "Измерения в норме",              };
const l10n_str_t l10n_str_link_ok[]                     = { "Link ok",                                  "Связь установлена",              };
const l10n_str_t l10n_str_zero_calibration[]            = { "Zero calibration",                         "Калибровка нуля",                };
const l10n_str_t l10n_str_span_calibration[]            = { "Span calibration",                         "Калибровка span",                };
const l10n_str_t l10n_str_restore_calibration[     ]    = { "Restore calibration",                      "Восстановление калибровки",      };
const l10n_str_t l10n_str_restore_zero_calibration[]    = { "Restore zero calibration",                 "Восстановление калибровки нуля", };
const l10n_str_t l10n_str_restore_span_calibration[]    = { "Restore span calibration",                 "Восстановление калибровки span", };
const l10n_str_t l10n_str_fatory_zero_calibration[]     = { "Factory zero calibration",                 "Заводская калибровка нуля",      };
const l10n_str_t l10n_str_factory_span_calibration[]    = { "Factory span calibration",                 "Заводская калибровка span",      };
const l10n_str_t l10n_str_update_zero_calibration[]     = { "Update zero calibration",                  "Обновление калибровки нуля",     };
const l10n_str_t l10n_str_update_span_calibration[]     = { "Update span calibration",                  "Обновление калибровки span",     };
const l10n_str_t l10n_str_char[]                        = { "Сharacteristic",                           "Характеристика",                 };
const l10n_str_t l10n_str_event_linearization[]         = { "linearization",                            "линеаризация",                   };
const l10n_str_t l10n_str_event_zero_offset[]           = { "Zero offset",                              "Смещение нуля",                  };
const l10n_str_t l10n_str_calibration_4mA[]             = { "Calibration 4mA",                          "Калибровка 4мА",                 };
const l10n_str_t l10n_str_calibration_20mA[]            = { "Calibration 20mA",                         "Калибровка 20мА",                };
const l10n_str_t l10n_str_event_factory_reset[]         = { "Factory reset",                            "Сброс настроек",                 };
const l10n_str_t l10n_str_verifycation_start[]          = { "Verifycation start",                       "Начало проверки",                };
const l10n_str_t l10n_str_verifycation_end[]            = { "Verifycation end",                         "Конец поверки",                  };
const l10n_str_t l10n_str_default[]                     = { "-",                                         "-",                             };
const l10n_str_t l10n_str_wrong_lang                    = { "---" };


/**
  * @brief  Localization string
  * @param  None
  * @retval None
  */

const   char *
l10n_str_get(                           const   l10n_lang_t     lang ,
                                        const   l10n_str_id_t   str_id )
{
    if( lang >= L10N_LANG_MAX )
    {
        return( l10n_str_wrong_lang );
    }

    switch( str_id )
    {
        case L10N_STR_ID_ALLOWED:               return( l10n_str_allowed[lang]           );
        case L10N_STR_ID_AUTO:                  return( l10n_str_auto[lang]              );
        case L10N_STR_ID_AUTO_CALIBRATION:      return( l10n_str_auto_calibration[lang]  );
        case L10N_STR_ID_AUTOLAUNCH:            return( l10n_str_autolaunch[lang]           );
        case L10N_STR_ID_CALIBRATION_CORRECTION:return( l10n_str_calibration_correction[lang]    );
        case L10N_STR_ID_BACK:                  return( l10n_str_back[lang]             );
        case L10N_STR_ID_CANCEL:                return( l10n_str_cancel[lang]           );
        case L10N_STR_ID_CALIBRATE:             return( l10n_str_calibrate[lang]        );
        case L10N_STR_ID_CALIBRATE_AL:          return( l10n_str_calibrate_al[lang]     );
        case L10N_STR_ID_CALIBRATE_AR:          return( l10n_str_calibrate_ar[lang]     );
        case L10N_STR_ID_CALIBRATION:           return( l10n_str_calibration[lang]      );
        case L10N_STR_ID_CALIBRATION_CHANNEL_1: return( l10n_str_calibration_channel1[lang]      );
        case L10N_STR_ID_CALIBRATION_CHANNEL_2: return( l10n_str_calibration_channel2[lang]      );        
        case L10N_STR_ID_CL420:                 return( l10n_str_cl420[lang]            );
        case L10N_STR_ID_COEFFICIENT:           return( l10n_str_coefficient[lang]      );
        case L10N_STR_ID_COMPLETED:             return( l10n_str_completed[lang]        );
        case L10N_STR_ID_CONSOLE:               return( l10n_str_console[lang]          );
        case L10N_STR_ID_CONSOLE_AL:            return( l10n_str_console_al[lang]       );
        case L10N_STR_ID_CONSOLE_AR:            return( l10n_str_console_ar[lang]       );
        case L10N_STR_ID_CONFIRM:               return( l10n_str_confirm[lang]          );
        case L10N_STR_ID_CONTINUOUS:            return( l10n_str_continuous[lang]       );
        case L10N_STR_ID_CORRECTION:            return( l10n_str_correction[lang]       );
        case L10N_STR_ID_CLOOP:                 return( l10n_str_cloop[lang]            );  
        case L10N_STR_ID_CLOOP_ERROR:           return( l10n_str_cloop_error[lang]      );
        case L10N_STR_ID_CLOOP_4MA:             return( l10n_str_cloop_4mA[lang]        );
        case L10N_STR_ID_CLOOP_20MA:            return( l10n_str_cloop_20mA[lang]       );
        case L10N_STR_ID_CLOOP_3MA:             return( l10n_str_cloop_3mA[lang]        );
        case L10N_STR_ID_CLOOP_22MA:            return( l10n_str_cloop_22mA[lang]       ); 
        case L10N_STR_ID_COARSE:                return( l10n_str_coarse[lang]           );  
        case L10N_STR_ID_CYCLE:                 return( l10n_str_cycle[lang]            );
        case L10N_STR_ID_CYCLE_CSM:             return( l10n_str_cycle_csm[lang]        );
        case L10N_STR_ID_CYCLE_CZM:             return( l10n_str_cycle_czm[lang]        );
        case L10N_STR_ID_CYCLE_PCSM:            return( l10n_str_cycle_pcsm[lang]       );
        case L10N_STR_ID_CYCLE_PCZM:            return( l10n_str_cycle_pczm[lang]       );
        case L10N_STR_ID_DATE:                  return( l10n_str_date[lang]             );
        case L10N_STR_ID_DAY:                   return( l10n_str_day[lang]              );
        case L10N_STR_ID_DAYS:                  return( l10n_str_days[lang]             );
        case L10N_STR_ID_DEBUG:                 return( l10n_str_debug[lang]            );
        case L10N_STR_ID_DIAGNOSTICS:           return( l10n_str_diagnostics[lang]      );
        case L10N_STR_ID_DISABLED:              return( l10n_str_disabled[lang]         );
        case L10N_STR_ID_DISCRETE_OUTPUT:       return( l10n_str_discrete_output[lang]  );
        case L10N_STR_ID_ENABLED:               return( l10n_str_enabled[lang]          );
        case L10N_STR_ID_ERROR:                 return( l10n_str_error[lang]            );
        case L10N_STR_ID_ENGLISH:               return( l10n_str_english[lang]          );
        case L10N_STR_ID_ENTER:                 return( l10n_str_enter[lang]            );
        case L10N_STR_ID_EVENT_SYSTEM_STARTUP:  return( l10n_str_system_startup[lang]   );                      
        case L10N_STR_ID_EVENT_LINK_ERROR:      return( l10n_str_link_error[lang]       );  
        case L10N_STR_ID_EVENT_LINK_OK:         return( l10n_str_link_ok[lang]          );
        case L10N_STR_ID_KEY_ON:                return( l10n_str_key_on[lang]          );
        case L10N_STR_ID_KEY_OFF:               return( l10n_str_key_off[lang]          );
        case L10N_STR_ID_EVENT_PID_TEMP_MEASURE_ERR:            return( l10n_str_temp_measure_err[lang]      );    
        case L10N_STR_ID_EVENT_PID_TEMP_MEASURE_OK:             return( l10n_str_temp_measure_ok[lang]       );   
        case L10N_STR_ID_EVENT_PID_TEMP_TOO_LOW_ERR:            return( l10n_str_temp_too_low[lang]          );   
        case L10N_STR_ID_EVENT_PID_TEMP_TOO_HIGH_ERR:           return( l10n_str_temp_too_high[lang]         );
        case L10N_STR_ID_EVENT_PID_NOT_IN_RANGE_LOW:            return( l10n_str_temp_not_in_range_low[lang] );        
        case L10N_STR_ID_EVENT_PID_NOT_IN_RANGE_HIGH:           return( l10n_str_temp_not_in_range_high[lang]); 
        case L10N_STR_ID_EVENT_VALUE_TOO_LOW_WAR:               return( l10n_str_value_too_low[lang]         );   
        case L10N_STR_ID_EVENT_VALUE_TOO_HIGH_WAR:              return( l10n_str_value_too_high[lang]         );           
        case L10N_STR_ID_EVENT_SENSOR_VALUE_OK:                 return( l10n_str_value_ok[lang]               );
        case L10N_STR_ID_EVENT_SENSOR_ZERO_CALIBRATION:         return( l10n_str_zero_calibration[lang]       ); 
        case L10N_STR_ID_EVENT_SENSOR_ZERO_FACTORY_UPDATE:      return( l10n_str_update_zero_calibration[lang]); 
        case L10N_STR_ID_EVENT_SENSOR_SPAN_CALIBRATION:         return( l10n_str_span_calibration[lang]       );    
        case L10N_STR_ID_EVENT_SENSOR_SPAN_FACTORY_UPDATE:      return( l10n_str_update_span_calibration[lang]); 
        case L10N_STR_ID_EVENT_SENSOR_RESTORE_CALIBRATION:      return( l10n_str_restore_calibration[lang]    ); 
        case L10N_STR_ID_EVENT_SENSOR_RESTORE_ZERO_CALIBRATION: return( l10n_str_restore_zero_calibration[lang]    ); 
        case L10N_STR_ID_EVENT_SENSOR_RESTORE_SPAN_CALIBRATION: return( l10n_str_restore_span_calibration[lang]    );                 
        case L10N_STR_ID_EVENT_SENSOR_ZERO_FACTORY_RESTORE:     return( l10n_str_fatory_zero_calibration[lang]); 
        case L10N_STR_ID_EVENT_SENSOR_CHAR:                     return( l10n_str_char[lang]                   );    
        case L10N_STR_ID_EVENT_SENSOR_LINEARIZATION:            return( l10n_str_event_linearization[lang]    ); 
        case L10N_STR_ID_EVENT_SENSOR_ZERO_OFFSET:              return( l10n_str_event_zero_offset[lang]      );   
        case L10N_STR_ID_EVENT_CALIBRATION_4MA:                 return( l10n_str_calibration_4mA[lang]        );   
        case L10N_STR_ID_EVENT_CALIBRATION_20MA:                return( l10n_str_calibration_20mA[lang]       );   
        case L10N_STR_ID_EVENT_FACTORY_RESET:                   return( l10n_str_event_factory_reset[lang]    );   
        case L10N_STR_ID_EVENT_VERIFYCATION_START:              return( l10n_str_verifycation_start[lang]     );   
        case L10N_STR_ID_EVENT_VERIFYCATION_STOP:               return( l10n_str_verifycation_end[lang]       );           
        case L10N_STR_ID_FACTORY_SETTINGS:      return( l10n_str_factory_settings[lang] );
        case L10N_STR_ID_FILTER:                return( l10n_str_filter[lang]           );
        case L10N_STR_ID_FILTER_CUTOFF:         return( l10n_str_filter_cutoff[lang]    );
        case L10N_STR_ID_FILTER_ORDER:          return( l10n_str_filter_order[lang]     );
        case L10N_STR_ID_FINE:                  return( l10n_str_fine[lang]             );        
        case L10N_STR_ID_FIRMWARE_VER:          return( l10n_str_firmware_ver[lang]     );
        case L10N_STR_ID_HOUR_METER:            return( l10n_str_hour_meter[lang]       );
        case L10N_STR_ID_HOUR:                  return( l10n_str_hour[lang]             );
        case L10N_STR_ID_INFO:                  return( l10n_str_info[lang]             );
        case L10N_STR_ID_INTERVAL:              return( l10n_str_interval[lang]         );
        case L10N_STR_ID_LANGUAGE:              return( l10n_str_language[lang]         );
        case L10N_STR_ID_LAUNCH:                return( l10n_str_launch[lang]           );
        case L10N_STR_ID_LOWER:                 return( l10n_str_lower[lang]            );
        case L10N_STR_ID_MEASURE:               return( l10n_str_measure[lang]          );
        case L10N_STR_ID_MEASURE_AL:            return( l10n_str_measure_al[lang]       );
        case L10N_STR_ID_MEASURE_AR:            return( l10n_str_measure_ar[lang]       );
        case L10N_STR_ID_MIDDLE:                return( l10n_str_middle[lang]           );
        case L10N_STR_ID_MIN:                   return( l10n_str_min[lang]              );
        case L10N_STR_ID_MINUTES:               return( l10n_str_minutes[lang]          );
        case L10N_STR_ID_MODE:                  return( l10n_str_mode[lang]             );
        case L10N_STR_ID_MDB_RELAY:             return( l10n_str_mdb_relay[lang]        );                
        case L10N_STR_ID_MONTH:                 return( l10n_str_month[lang]            );
        case L10N_STR_ID_OVER_MODBUS:           return( l10n_str_over_modbus[lang]      );
        case L10N_STR_ID_PASSWORD:              return( l10n_str_password[lang]         );
        case L10N_STR_ID_PASSWORD_RESET:        return( l10n_str_password_reset[lang]   );
        case L10N_STR_ID_POINT_LOW:             return( l10n_str_point_low[lang]        );
        case L10N_STR_ID_PRESSURE:              return( l10n_str_pressure[lang]         );
        case L10N_STR_ID_PROCESS:               return( l10n_str_process[lang]          );
        case L10N_STR_ID_PROHIBITED:            return( l10n_str_prohibited[lang]       );
        case L10N_STR_ID_PURGE:                 return( l10n_str_purge[lang]            );
        case L10N_STR_ID_RANGE:                 return( l10n_str_range[lang]            );
        case L10N_STR_ID_REMOTELY:              return( l10n_str_remotely[lang]         );
        case L10N_STR_ID_RELAY:                 return( l10n_str_relay[lang]            );     
        case L10N_STR_ID_RELAY_1:               return( l10n_str_relay_1[lang]          );  
        case L10N_STR_ID_RELAY_2:               return( l10n_str_relay_2[lang]          );   
        case L10N_STR_ID_RELAY_STATE_NO:        return( l10n_str_relay_state_no[lang]   );  
        case L10N_STR_ID_RELAY_STATE_NC:        return( l10n_str_relay_state_nc[lang]   );         
        case L10N_STR_ID_RESTORE:               return( l10n_str_restore[lang]          );
        case L10N_STR_ID_RUNS_COUNTER:          return( l10n_str_runs_counter[lang]     );
        case L10N_STR_ID_RUSSIAN:               return( l10n_str_russian[lang]          );
        case L10N_STR_ID_SCALE:                 return( l10n_str_scale[lang]            );
        case L10N_STR_ID_SEC:                   return( l10n_str_sec[lang]              );
        case L10N_STR_ID_SECONDS:               return( l10n_str_seconds[lang]          );
        case L10N_STR_ID_SELECT:                return( l10n_str_select[lang]           );
        case L10N_STR_ID_SENSOR:                return( l10n_str_sensor[lang]           );
        case L10N_STR_ID_SERIAL_NUM:            return( l10n_str_serial_num[lang]       );
        case L10N_STR_ID_SERVICE:               return( l10n_str_service[lang]          );
        case L10N_STR_ID_SETUP:                 return( l10n_str_setup[lang]            );
        case L10N_STR_ID_SETUP_AL:              return( l10n_str_setup_al[lang]         );
        case L10N_STR_ID_SETUP_AR:              return( l10n_str_setup_ar[lang]         );
        case L10N_STR_ID_SLOPE:                 return( l10n_str_slope[lang]            );
        case L10N_STR_ID_SMOOTHING:             return( l10n_str_smoothing[lang]        );
        case L10N_STR_ID_SPECIFY:               return( l10n_str_specify[lang]          );
        case L10N_STR_ID_SPAN:                  return( l10n_str_span[lang]             );
        case L10N_STR_ID_STATE:                 return( l10n_str_state[lang]            );                
        case L10N_STR_ID_SUCCESS:               return( l10n_str_success[lang]          );
        case L10N_STR_ID_SYSTEM:                return( l10n_str_system[lang]           );
        case L10N_STR_ID_TEMPERATURE:           return( l10n_str_temperature[lang]      );
        case L10N_STR_ID_TIMINGS:               return( l10n_str_timings[lang]          );
        case L10N_STR_ID_THRESHOLD:             return( l10n_str_threshold[lang]        );
        case L10N_STR_ID_THRESHOLD_1:           return( l10n_str_threshold_1[lang]      );
        case L10N_STR_ID_THRESHOLD_2:           return( l10n_str_threshold_2[lang]      );
        case L10N_STR_ID_TIME:                  return( l10n_str_time[lang]             );
        case L10N_STR_ID_UPPER:                 return( l10n_str_upper[lang]            );
        case L10N_STR_ID_VALUE:                 return( l10n_str_value[lang]            );
        case L10N_STR_ID_YEAR:                  return( l10n_str_year[lang]             );
        case L10N_STR_ID_ZERO:                  return( l10n_str_zero[lang]             );
        default:                                return( l10n_str_default[lang]          );
    }
}


const   char **
l10n_lang_list_get( void )
{
        return( (char const **) l10n_list_language );
}

size_t
l10n_lang_countof_get( void )
{
        return( sizeof( l10n_list_language ) / sizeof( l10n_str_t ) );
}

const   char **
l10n_relay_mode_list_get( int lang )
{
  if (lang) return( (char const **) l10n_list_relay_mode_ru ); 
  
  return( (char const **) l10n_list_relay_mode_en ); 
}

size_t
l10n_mode_countof_get( void )
{
        return( sizeof( l10n_list_relay_mode_ru ) / sizeof( l10n_str_t ) );
}
