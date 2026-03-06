#1
full_data<-read.csv("D:/Subject/XSTK/jfk_weather_sample.csv")
observations<-nrow(full_data)
print(observations)

#2
uni<-unique(full_data$HOURLYPrecip)
numuni<- length(uni)
print(numuni)

full_data$HOURLYPrecip<-as.character(full_data$HOURLYPrecip)
count_T<-sum(full_data$HOURLYPrecip=="T", na.rm = TRUE)
print(count_T)
count_S<-sum(grep("s$",full_data$HOURLYPrecip),na.rm = TRUE)
print(count_S)

#3
col_select<-c('HOURLYDewPointTempF','HOURLYStationPressure','HOURLYPrecip')
new_data<-full_data[,col_select]

obs_total<-nrow(full_data)
na_dewpoint_count<-sum(is.na(full_data$HOURLYDewPointTempF))
na_dewpoint_perc<-(na_dewpoint_count/obs_total)*100

na_stationpressure_count<-sum(is.na(full_data$HOURLYStationPressure))
na_stationpressure_perc<-(na_stationpressure_count/obs_total)*100

full_data$HOURLYPrecip<-as.character(full_data$HOURLYPrecip)
na_precip_count<-sum(is.na(full_data$HOURLYPrecip))
na_precip_perc<-(na_precip_count/obs_total)*100

print(na_dewpoint_count)
print(na_dewpoint_perc)

print(na_stationpressure_count)
print(na_stationpressure_perc)

print(na_precip_count)
print(na_precip_perc)

new_data_cleaned<-na.omit(new_data)
print(nrow(new_data_cleaned))

Q_col<-c('HOURLYDewPointTempF','HOURLYStationPressure')
Q_temp<-full_data[,Q_col]

Q_temp$HOURLYDewPointTempF<-as.numeric(as.character(Q_temp$HOURLYDewPointTempF))
Q_temp$HOURLYStationPressure<-as.numeric(as.character(Q_temp$HOURLYStationPressure))

temp_data_cleaned<-na.omit(Q_temp)
Q_dew<-quantile(temp_data_cleaned$HOURLYDewPointTempF,probs = c(0.25,0.5,0.75),na.rm = TRUE)
Q_sta<-quantile(temp_data_cleaned$HOURLYStationPressure,probs=c(0.25,0.5,0.75),na.rm = TRUE)
print(paste(Q_dew[1],";",Q_dew[2],";",Q_dew[3]))
print(paste(Q_sta[1],";",Q_sta[2],";",Q_sta[3]))

col_cor<-c('HOURLYPrecip','HOURLYDewPointTempF','HOURLYStationPressure')
temp_cor<-full_data[col_cor]

temp_cor$HOURLYPrecip<-as.character(temp_cor$HOURLYPrecip)
temp_cor$HOURLYPrecip[temp_cor$HOURLYPrecip=="T"]
