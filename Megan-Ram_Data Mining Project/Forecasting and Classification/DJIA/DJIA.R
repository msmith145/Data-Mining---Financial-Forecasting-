
library("xlsx")
mydata <- read.xlsx("C:/Users/megan/Documents/myexcel.xlsx", 1)
mydata <- read.xlsx("C:/Users/megan/Documents/myexcel.xlsx", sheetName = "DJIA")

summary(mydata)

hist(mydata$VALUE, sub = "Histogram of DJIA dataset")
x11()

plot(density(mydata$VALUE), sub = "Density of DJIA dataset")
x11()

plot(mydata$DATE, mydata$VALUE, sub = "DJIA dataset")
x11()

pairs(mydata)
x11()

require(xts)
class(mydata)

mydata$DATE <- as.Date(as.character(mydata$DATE),format="%Y-%m-%d")

matrix_xts <- as.xts(mydata, mydata$DATE)
mydata2 <- head(mydata, n = 2350)
df_xts2 <- as.xts(mydata[,-1], order.by=mydata[,1])
df_xts <- as.xts(mydata2[,-1], order.by=mydata2[,1])

require(forecast)

x.ts2 = ts(df_xts2, freq=365)
x.ts = ts(df_xts, freq=365)

plot(x.ts2,col=3,ylim=range(6000:20000), xaxt="n",ylab = "Value", sub = "DJIA dataset" )
x11()

plot(forecast(arima(x.ts,order=c(2,0,2)),10), xaxt="n", sub ="Forcast of DJIA dataset", ylab = "Value", xlab = "Time")
x11()

summary(forecast(arima(x.ts,order=c(2,0,2)), 10))

f <- decompose(x.ts2)

plot(f$figure, type="b", xaxt="n", xlab="Time",col=10)
x11()

plot(f, xaxt="n")
x11()

mydata <- head(mydata, n=64)

Time <- seq(as.Date("04-01-2005", format = "%m-%d-%Y"), by = "days", length = 64)

mydata[1] <- as.numeric(Time) 

library(party)

ct <- ctree(VALUE ~ (VALUE+1), data=mydata)

plot(ct)








