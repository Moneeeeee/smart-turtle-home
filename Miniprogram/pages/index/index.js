// index.js
const defaultAvatarUrl = 'https://mmbiz.qpic.cn/mmbiz/icTdbqWNOwNRna42FI242Lcia07jQodd2FJGIYQfG0LAJGFxM4FbnQP6yfMxBgJ0F3YRqJCJ1aPAK2dQagdusBZg/0'

const app = getApp()

const { connect } = require('../../utils/mqtt')

const mqttHost = 'broker.emqx.io' // mqtt服务器域名
const mqttPort = 8084 // mqtt服务器端口

const deviceSubTopic = '/mysmartturtle/sub' // 设备订阅topic
const devicePubTopic = '/mysmartturtle/pub' // 设备发布topic

const mpSubTopic = devicePubTopic
const mpPubTopic = deviceSubTopic
// 在Page({})函数外定义 threshold 变量
let tem_threshold = 40; // 默认阈值为50
let mq2_threshold = 300; // 默认阈值为50
Page({
  data: {
    client: null,
    Temp:0,
    TDS:0,
    Lumen:0,
    Weight:0,
    UVB:false,
    UVA:false,
    Pump:false,
    WATER:false,
    Eat:0,
    tem_threshold:0,
    TDS_threshold:0,
    Lumen_threshold:0,
  },

  onUVAChange(event){
    const that  = this;
    const sw = event.detail.value;
    that.setData({ UVA: sw });
    // console.log(event.detail,value);
    // that.setData({LED:sw})
      that.data.client.publish(mpPubTopic, JSON.stringify({
        target: "UVA",
        value: sw ? 1 : 0
      }), function (err) {
        if (!err) {
          console.log('成功下发指令：' + (sw ? 'UVA:开启' : 'UVA:关闭'));
        }
    });
  },
  onUVBChange(event){
    const that  = this;
    const sw = event.detail.value;
    that.setData({ UVB: sw });
    // console.log(event.detail,value);
    // that.setData({LED:sw})
      that.data.client.publish(mpPubTopic, JSON.stringify({
        target: "UVB",
        value: sw ? 1 : 0
      }), function (err) {
        if (!err) {
          console.log('成功下发指令：' + (sw ? 'UVB:开启' : 'UVB:关闭'));
        }
    });
  },


  onEATClick(event){
    const that = this;
    // 只发送信号，不需要更改Steer的值
    that.data.client.publish(mpPubTopic, JSON.stringify({
      target: "Eat",
      value: 1 // 或者您可以选择发送一个特定的信号，代表“单次操作”
    }), function (err) {
      if (!err) {
        console.log('成功下发单次投喂指令');
        // 如果需要反馈到界面，可以在这里修改界面元素的显示
        // 例如，显示一个Toast消息来告知用户
        wx.showToast({
          title: '投喂成功',
          icon: 'success',
          duration: 2000
        });
      } else {
        // 处理错误情况
      }
    });
  },

  onPumpChange(event){
    const that  = this;
    const sw = event.detail.value;
    that.setData({ Pump: sw });
    // console.log(event.detail,value);
    // that.setData({LED:sw})
      that.data.client.publish(mpPubTopic, JSON.stringify({
        target: "Pump",
        value: sw ? 1 : 0
      }), function (err) {
        if (!err) {
          console.log('成功下发指令：' + (sw ? '开启换水' : '关闭换水'));
        }
    });
  },

  onTEMThresholdChange(event) {
    const that = this;
    const temperature_Thresold = event.detail.value;
    // 将 number 类型的数据转换为 uint8 类型
    // const tem_threshold_uint8 = Math.round(tem_threshold_read); // 使用 Math.round() 进行四舍五入取整
    that.setData({ temperature_Thresold: temperature_Thresold });
    // 发送 uint8 类型的数据到 STM32 端
    that.data.client.publish(mpPubTopic, JSON.stringify({
      target: "temperature_Thresold",
      value: temperature_Thresold // 发送 uint8 类型的数据
    }), function (err) {
      if (!err) {
        console.log('成功下发温度阈值:', temperature_Thresold);
      }
    });
  },
  

  onTDSThresholdChange(event) {
    const that = this;
    const TDS_Thresold = event.detail.value;
    that.setData({ TDS_Thresold: TDS_Thresold });
    // console.log(event.detail,value);
    // that.setData({LED:sw})
      that.data.client.publish(mpPubTopic, JSON.stringify({
        target: "TDS_Thresold",
        value: TDS_Thresold
      }), function (err) {
        if (!err) {
          console.log('成功下发TDS阈值:', TDS_Thresold);
        }
    });
  },
  onLumenThresholdChange(event) {
    const that = this;
    const Lumen_Thresold = event.detail.value;
    that.setData({ Lumen_Thresold: Lumen_Thresold });
    // console.log(event.detail,value);
    // that.setData({LED:sw})
      that.data.client.publish(mpPubTopic, JSON.stringify({
        target: "Lumen_Thresold",
        value: Lumen_Thresold
      }), function (err) {
        if (!err) {
          console.log('成功下发光照阈值:', Lumen_Thresold);
        }
    });
  },
  
  //事件处理函数
  onShow() {
    const that = this
    that.setData({
      client: connect(`wxs://${mqttHost}:${mqttPort}/mqtt`)
    });
    that.data.client.on('connect', function () {
      console.log('成功连接到MQTT服务器');
      wx.showToast({
        title: '连接成功',
        icon: 'success',
        mask: true
      });

      that.data.client.subscribe(mpSubTopic, function (err) {
        if (err) {
          console.error('订阅设备上行数据Topic失败:', err);
        } else {
          console.log('成功订阅设备上行数据Topic');
        }
  
      });
    });
  
    that.data.client.on('message',function(topic,message){
      console.log(topic);
      // console.log(message);
      //message是十六进制的buffer流

      let dataFromDev = {};
      try{
        dataFromDev = JSON.parse(message)
        console.log(dataFromDev);

        that.setData({
          Temp:dataFromDev.Temp,
          TDS:dataFromDev.TDS,
          Lumen:dataFromDev.Lumen,
          Weight:dataFromDev.Weight,
          Weight_Flag:dataFromDev.Weight_Flag,
          Eat:dataFromDev.Eat,
          Water:dataFromDev.Water,
          UVA:dataFromDev.UVA,
          UVB:dataFromDev.UVB,
        });

      }catch(error){
        console.log(error);
        console.log('JSON解析失败',error);
      }

    });
  }
});
