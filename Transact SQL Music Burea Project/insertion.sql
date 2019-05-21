-------------------------------------------------------------------------------
-- INSERTION USING PROCEDURES
-------------------------------------------------------------------------------

-- Legal clients
EXEC add_legal_client '9120100001', 'ул. Пушкина, д. 1',      '0000000000001'; -- 0
EXEC add_legal_client '9120100002', 'ул. Мира, д. 2',         '0000000000002'; -- 1
EXEC add_legal_client '9120100003', 'ул. Химиков, д. 12',     '0000000000003'; -- 2
EXEC add_legal_client '9120100004', 'ул. Рейнджеров, д. 42',  '0000000000004'; -- 3
EXEC add_legal_client '9120100005', 'пр. Просвещения, д. 13', '0000000000005'; -- 4

-- Private clients
EXEC add_private_client '9120000001', 'пр. Ветеранов, д. 1',    '4012000001'; -- 5
EXEC add_private_client '9120000002', 'пр. Большевиков, д. 2',  '4012000002'; -- 6
EXEC add_private_client '9120000003', 'ул. Меньшевиков, д. 3',  '4012000003'; -- 7
EXEC add_private_client '9120000004', 'ул. Достоевского, д. 4', '4012000004'; -- 8
EXEC add_private_client '9120000005', 'ул. Матросова, д.5',     '4012000005'; -- 9

-- Instruments
EXEC add_instrument_class 'Струнные';
EXEC add_instrument_type 'Щипковые',  'Струнные';
EXEC add_instrument_type 'Смычковые', 'Струнные';
EXEC add_instrument_subtype 'Акустическая гитара', 'Щипковые';
EXEC add_instrument_subtype 'Арфа',                'Щипковые';
EXEC add_instrument_subtype 'Скрипка',             'Смычковые';

EXEC add_instrument_class 'Духовые';
EXEC add_instrument_type 'Деревянные', 'Духовые';
EXEC add_instrument_type 'Медные',     'Духовые';
EXEC add_instrument_subtype 'Оркестровая флейта', 'Деревянные';
EXEC add_instrument_subtype 'Тромбон',            'Медные';
EXEC add_instrument_subtype 'Саксофон',           'Медные';

EXEC add_instrument_class 'Ударные';
EXEC add_instrument_type 'Мембранофоны', 'Ударные';
EXEC add_instrument_type 'Идиофоны',     'Ударные';
EXEC add_instrument_type 'Струнные',     'Ударные';
EXEC add_instrument_type 'Перкуссия',    'Ударные';
EXEC add_instrument_subtype 'Барабаны',    'Мембранофоны';
EXEC add_instrument_subtype 'Треугольник', 'Идиофоны';
EXEC add_instrument_subtype 'Фортепиано',  'Струнные';
EXEC add_instrument_subtype 'Тамбурин',    'Перкуссия';


EXEC add_instrument 'Акустическая гитара', 'Когда-то на ней играл Цой'; -- 0
EXEC add_instrument 'Акустическая гитара'; -- 1
EXEC add_instrument 'Арфа', 'Белая'; -- 2
EXEC add_instrument 'Скрипка', 'Абсолютно новая'; -- 3
EXEC add_instrument 'Скрипка'; -- 4
EXEC add_instrument 'Оркестровая флейта', 'Чёрная'; -- 5
EXEC add_instrument 'Тромбон', 'Небольшие царапины'; -- 6
EXEC add_instrument 'Тромбон'; -- 7
EXEC add_instrument 'Саксофон'; -- 8
EXEC add_instrument 'Саксофон'; -- 9
EXEC add_instrument 'Барабаны', 'Три разноразмерных'; -- 10
EXEC add_instrument 'Треугольник'; -- 11
EXEC add_instrument 'Тамбурин'; -- 12


EXEC add_configuration 0, 'Виктор Цой', '1985-04-01';
EXEC add_configuration 0, 'Олег', '2011-01-05';
EXEC add_configuration 1, 'Иван', '2013-02-06';
EXEC add_configuration 2, 'Николай', '2012-01-01';
EXEC add_configuration 3, 'Александр', '2010-01-05';
EXEC add_configuration 8, 'Алексей', '2000-01-05';
EXEC add_configuration 9, 'Случайный мужчина в костюме', '2012-12-30';
EXEC add_configuration 9, 'Олег';


EXEC add_price 0, 1, 0, 100;
EXEC add_price 0, 5, 0, 75;
EXEC add_price 0, 1, 1, 90;
EXEC add_price 0, 5, 1, 70;

EXEC add_price 1, 1, 0, 80;
EXEC add_price 1, 1, 1, 70;

EXEC add_price 2, 1, 0, 100;
EXEC add_price 2, 1, 1, 90;

EXEC add_price 3, 1, 0, 150;
EXEC add_price 3, 1, 1, 120;

EXEC add_price 4, 1, 0, 130;
EXEC add_price 4, 1, 1, 110;

EXEC add_price 5, 1, 0, 80;
EXEC add_price 5, 1, 1, 80;

EXEC add_price 6, 1, 0, 100;
EXEC add_price 6, 1, 1, 100;

EXEC add_price 7, 1, 0, 120;
EXEC add_price 7, 1, 1, 120;

EXEC add_price 8, 1, 0, 80;
EXEC add_price 8, 1, 1, 70;

EXEC add_price 9, 1, 0, 80;
EXEC add_price 9, 1, 1, 70;

EXEC add_price 10, 1, 0, 500;
EXEC add_price 10, 1, 1, 400;

EXEC add_price 11, 1, 0, 50;
EXEC add_price 11, 1, 1, 40;

EXEC add_price 12, 1, 0, 60;
EXEC add_price 12, 1, 1, 60;


EXEC add_lease 0, 0, '2019-03-06', '2019-03-07';
EXEC add_lease 0, 0, '2019-03-08', '2019-03-10';
EXEC add_lease 1, 0, '2019-03-11', '2019-03-16';
EXEC add_lease 2, 0, '2019-03-17', '2019-03-24';

EXEC add_lease 5, 1, '2019-03-10', '2019-03-11';
EXEC add_lease 6, 1, '2019-03-12', '2019-03-20';

EXEC add_lease 2, 2, '2019-03-10', '2019-03-25';

EXEC add_lease 8, 4, '2019-02-15', '2019-03-15';

EXEC add_lease 9, 6, '2019-03-10', '2019-04-20';

EXEC add_lease 1, 7, '2019-03-10', '2019-03-11';
EXEC add_lease 1, 7, '2019-03-12', '2019-03-15';

EXEC add_lease 5, 9, '2019-02-01', '2019-03-21';

EXEC add_lease 7, 12, '2019-03-12', '2019-03-20';


-------------------------------------------------------------------------------
-- TESTING OTHER FUNCTIONS
-------------------------------------------------------------------------------

EXEC inspect_instrument 0, '2019-03-12';

EXEC rate_lease 0, '2019-03-06', 9;
EXEC rate_lease 0, '2019-03-08', 10;

DECLARE @test_float FLOAT;
EXEC @test_float = get_rating 0;
PRINT @test_float; -- Should be (9 + 10) / 2