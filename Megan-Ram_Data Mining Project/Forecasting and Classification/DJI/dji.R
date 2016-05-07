
library("xlsx")
mydata <- read.xlsx("C:/Users/megan/Documents/DJIndex.xlsx", 1)
mydata <- read.xlsx("C:/Users/megan/Documents/DJIndex.xlsx", sheetName = "DJI")

summary(mydata)

hist(mydata$PERCENT_CHANGE_NEXT_WEEKS_PRICE, sub = "Histogram of DJI dataset" )
x11()

plot(density(mydata$PERCENT_CHANGE_NEXT_WEEKS_PRICE), sub = "Density of DJI dataset")
x11()

plot(mydata$DATE, mydata$PERCENT_CHANGE_NEXT_WEEKS_PRICE, sub = "DJI dataset")
x11()

pairs(mydata)
x11()

require(xts)
class(mydata)

mydata$DATE <- as.Date(as.character(mydata$DATE),format="%Y-%m-%d")
matrix_xts <- as.xts(mydata, mydata$DATE)


x2 <- mydata[mydata[, "STOCK"] == "AA",]
x3 <- x2[x2[,"QUARTER"] == "1",]
x4 <- x2[x2[,"QUARTER"] == "2",]

rateOfReturn2 <- x2$PERCENT_CHANGE_NEXT_WEEKS_PRICE
rateOfReturn <- x3$PERCENT_CHANGE_NEXT_WEEKS_PRICE

Time2 <- seq(as.Date("07/01/2011", format = "%d/%m/%Y"), by = "weeks", length = 25)
Time <- seq(as.Date("07/01/2011", format = "%d/%m/%Y"), by = "weeks", length = 12)

plot(Time, rateOfReturn, type = "b", sub = "AA STOCK - QUARTER 1", ylab = "PERCENT_CHANGE_NEXT_WEEKS_PRICE")
x11()

plot(Time2, rateOfReturn2, type = "b", sub = "AA STOCK - QUARTER 1 and 2", ylab = "PERCENT_CHANGE_NEXT_WEEKS_PRICE")
x11()

require(forecast)

df_xts2 <- as.xts(rateOfReturn, order.by=Time)

x.ts2 = ts(df_xts2, freq=365)

m = ts(x.ts2, start=c(2011), frequency=365)

plot(forecast(arima(m,order=c(2,0,2)), 10), xaxt="n",type = "b", xlab = "Time",ylab = "PERCENT_CHANGE_NEXT_WEEKS_PRICE", sub = "Forecast of AA STOCK" )
x11()

summary(forecast(arima(m,order=c(2,0,2)), 10))

mydata2 <- head(mydata, n=108)

Time2 <- seq(as.Date("01-07-2011", format = "%m-%d-%Y"), by = "weeks", length = 108)

mydata2[3] <- as.numeric(Time2) 

library(party)

ct <- ctree(STOCK ~ QUARTER+OPEN+HIGH+LOW+CLOSE+VOLUME+PERCENT_CHANGE_NEXT_WEEKS_PRICE+PREVIOUS_WEEKS_VOLUME+PERCENT_CHANGE_PRICE+PERCENT_CHANGE_VOLUME_OVER_LAST_WK+NEXT_WEEKS_OPEN+NEXT_WEEKS_CLOSE+DAYS_TO_NEXT_DIVIDENE+PERCENT_RETURN_NEXT_DIVIDEND, data=mydata2)

plot(ct)
x11()

library("rpart")
library("rpart.plot")

mydata <- head(mydata, n=12)

fit <- rpart(ACTION ~ ., method="class", data=mydata, control=rpart.control(minsplit=1))

plot(fit, uniform=TRUE, main="Decision Tree - Buy or Sell or Hold?")

text(fit, use.n=TRUE, all=TRUE, cex=1)
