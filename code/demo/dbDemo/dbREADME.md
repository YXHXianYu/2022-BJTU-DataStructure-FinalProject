README——rank

### 食用说明

用于rank的接口使用参考demo

### 数据结构说明

1.单个玩家的数据

KEY-VALUE的集合，例如{ID-1，NAME-yxh，SCORE-100}，以QHash<QString,QString>存储

2.所有玩家数据

QList<QHash<QString,QString>>

### 接口中对如下操作有基于QDebug的报错

1.向db中添加新数据时（addData操作），若新数据的id与db中已有id重复，则报错（ps.名字可以重复）

2.删除不存在的数据

3.其他离谱问题比如数据库自身的异常打不开了等等