﻿//
// Created by ydrml on 2019/2/22.
//

#ifndef PM1_SDK_API_H
#define PM1_SDK_API_H

#ifdef _MSC_VER
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif // _MSC_VER

#include <string>
#include <vector>

namespace autolabor {
    namespace pm1 {
        /**
         * 顶层函数执行的结果
         */
        template<class t>
        struct result {
            /**
             * 错误信息
             *
             * 若成功，空字符串
             * 若失败，错误信息
             */
            std::string error_info;
            
            /**
             * 返回值
             *
             * 若成功，返回值
             * 若失败，未定义
             */
            t value;
            
            /**
             * 判断顶层函数是否成功
             */
            explicit operator bool() const {
                return error_info.empty();
            }
        };
        
        /**
         * 特化：无返回值的顶层函数执行的结果
         */
        template<>
        struct result<void> {
            /**
             * 错误信息
             *
             * 若成功，空字符串
             * 若失败，错误信息
             */
            std::string error_info;
            
            /**
             * 判断顶层函数是否成功
             */
            explicit operator bool() const {
                return error_info.empty();
            }
        };
        
        /**
         * 用于访问底盘参数的标识符
         */
        enum class parameter_id : uint32_t {
            width,           // 宽度（轮间距）
            length,          // 长度（轴间距）
            wheel_radius,    // 轮半径
            max_wheel_speed, // 最大动力轮角速度
            max_v,           // 最大底盘线速度
            max_w,           // 最大底盘角速度
            optimize_width,  // 优化函数半宽度
            acceleration,    // 最大动力轮角加速度
        };
        
        /**
         * 表示底盘状态的标识符
         */
        enum class chassis_state : uint8_t {
            offline  = 0x00, // 未知（离线）
            unlocked = 0x01, // 未锁定
            error    = 0x7f, // 已连接但异常
            locked   = 0xff  // 已锁定
        };
        
        /**
         * 里程，里程计读数
         */
        struct odometry {
            double x, y, yaw, vx, vy, w;
        };
        
        /**
         * 初始化
         *
         * @param port     串口名字
         * @param progress 进度
         */
        DLL_EXPORT result<std::string>
        initialize(const std::string &port = "",
                   double *progress = nullptr);
        
        /**
         * 关闭
         */
        DLL_EXPORT result<void>
        shutdown();
        
        /**
         * 获取底盘参数默认值
         *
         * @param id 要获取的参数项
         * @return 参数默认值
         */
        DLL_EXPORT double
        get_defualt_parameter(parameter_id id);
        
        /**
         * 获取底盘参数当前值
         *
         * @param id 要获取的参数项
         * @return 参数当前值
         */
        DLL_EXPORT result<double>
        get_parameter(parameter_id id);
        
        /**
         * 设置底盘参数
         *
         * @param id    要设置的参数项
         * @param value 参数值
         */
        DLL_EXPORT result<void>
        set_parameter(parameter_id id, double value);
        
        /**
         * 重置参数
         *
         * @param id 要重置的参数项
         */
        DLL_EXPORT result<void>
        reset_parameter(parameter_id id);
        
        /**
         * 读取里程计
         *
         * @return 里程计值或异常信息
         */
        DLL_EXPORT result<odometry>
        get_odometry();
        
        /**
         * 清除里程计累计值
         */
        DLL_EXPORT result<void>
        reset_odometry();
        
        /**
         * 锁定底盘
         */
        DLL_EXPORT result<void>
        lock();
        
        /**
         * 解锁底盘
         */
        DLL_EXPORT result<void>
        unlock();
        
        /**
         * 检查底盘状态
         *
         * @returns 机器人状态
         */
        DLL_EXPORT chassis_state
        check_state();
        
        /**
         * 按物理模型描述控制机器人运行
         *
         * @param speed  轮速
         * @param rudder 舵轮转角
         */
        DLL_EXPORT result<void>
        drive_physical(double speed, double rudder);
        
        /**
         * 按差动模型描述控制机器人运行
         *
         * @param left  左轮角速度
         * @param right 右轮角速度
         */
        DLL_EXPORT result<void>
        drive_wheels(double left, double right);
        
        /**
         * 按运动模型描述控制机器人运行
         *
         * @param v 线速度
         * @param w 角速度
         */
        DLL_EXPORT result<void>
        drive_velocity(double v, double w);
        
        /**
         * 按运动模型描述控制机器人运行（缩写）
         *
         * @param v 线速度
         * @param w 角速度
         */
        DLL_EXPORT result<void>
        drive(double v, double w);
        
        /**
         * 直线行驶
         *
         * @param speed    线速度
         * @param meters   距离
         * @param progress 进度
         */
        DLL_EXPORT result<void>
        go_straight(double speed,
                    double meters,
                    double *progress = nullptr);
        
        /**
         * 走直线
         *
         * @param speed    线速度
         * @param seconds  时间
         * @param progress 进度
         */
        DLL_EXPORT result<void>
        go_straight_timing(double speed,
                           double seconds,
                           double *progress = nullptr);
        
        /**
         * 原地转
         *
         * @param speed    角速度
         * @param rad      弧度
         * @param progress 进度
         */
        DLL_EXPORT result<void>
        turn_around(double speed,
                    double rad,
                    double *progress = nullptr);
        
        /**
         * 原地转
         *
         * @param speed    角速度
         * @param seconds  时间
         * @param progress 进度
         */
        DLL_EXPORT result<void>
        turn_around_timing(double speed,
                           double seconds,
                           double *progress = nullptr);
        
        /**
         * 走圆弧
         *
         * @param v        线速度
         * @param r        转弯半径
         * @param s        路程约束
         * @param progress 进度
         */
        DLL_EXPORT result<void>
        go_arc_vs(double v,
                  double r,
                  double s,
                  double *progress = nullptr);
        /**
         * 走圆弧
         *
         * @param v        线速度
         * @param r        转弯半径
         * @param a        转向约束
         * @param progress 进度
         */
        DLL_EXPORT result<void>
        go_arc_va(double v,
                  double r,
                  double a,
                  double *progress = nullptr);
        /**
         * 走圆弧
         *
         * @param w        角速度
         * @param r        转弯半径
         * @param s        路程约束
         * @param progress 进度
         */
        DLL_EXPORT result<void>
        go_arc_ws(double w,
                  double r,
                  double s,
                  double *progress = nullptr);
        /**
         * 走圆弧
         *
         * @param w        角速度
         * @param r        转弯半径
         * @param a        转向约束
         * @param progress 进度
         */
        DLL_EXPORT result<void>
        go_arc_wa(double w,
                  double r,
                  double a,
                  double *progress = nullptr);
        /**
         * 走圆弧
         *
         * @param v        线速度
         * @param r        转弯半径
         * @param t        时间约束
         * @param progress 进度
         */
        DLL_EXPORT result<void>
        go_arc_vt(double v,
                  double r,
                  double t,
                  double *progress = nullptr);
        /**
         * 走圆弧
         *
         * @param w        角速度
         * @param r        转弯半径
         * @param t        时间约束
         * @param progress 进度
         */
        DLL_EXPORT result<void>
        go_arc_wt(double w,
                  double r,
                  double t,
                  double *progress = nullptr);
        
        /**
         * 暂停动作执行
         */
        DLL_EXPORT void
        pause();
        
        /**
         * 恢复动作执行
         */
        DLL_EXPORT void
        resume();
        
        /**
         * 检查是否暂停
         *
         * @return 是否暂停
         */
        DLL_EXPORT bool
        is_paused();
        
        /**
         * 取消所有正在执行的动作
         */
        DLL_EXPORT void
        cancel_action();
        
        /**
         * 计算动作的空间尺度
         *
         * @param spatium 路程（弧长）
         * @param angle 角度（圆心角）
         */
        DLL_EXPORT double
        calculate_spatium(double spatium, double angle);
        
        /**
         * 控制机器人按空间约束运行指定动作
         *
         * @param v        线速度
         * @param w        角速度
         * @param s        空间约束
         * @param progress 进度
         */
        DLL_EXPORT result<void>
        drive_spatial(double v,
                      double w,
                      double s,
                      double *progress = nullptr);
        
        /**
         * 控制机器人按时间约束运行指定动作
         *
         * @param v        线速度
         * @param w        角速度
         * @param t        时间约束
         * @param progress 进度
         */
        DLL_EXPORT result<void>
        drive_timing(double v,
                     double w,
                     double t,
                     double *progress = nullptr);
        
        /**
         * 调整后轮零位
         *
         * @param offset   线速度
         * @param progress 进度
         */
        DLL_EXPORT result<void>
        adjust_rudder(double offset,
                      double *progress = nullptr);
        
        /**
         * 获取串口列表
         *
         * @return 全部串口的名字列表
         */
        DLL_EXPORT std::vector<std::string>
        serial_ports();
        
        /**
         * 阻塞线程延时
         *
         * @param seconds 时间
         */
        DLL_EXPORT void
        delay(double seconds);
    } // namespace pm1
} // namespace autolabor

#undef DLL_EXPORT

#endif //PM1_SDK_API_H
