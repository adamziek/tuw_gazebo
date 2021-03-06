#include <ros/ros.h>
#include <tuw_gazebo_plugins/gazebo_ros_utils.h>
#include <tuw_gazebo_plugins/tire_model.h>
#include <gazebo/common/common.hh>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <tuw_gazebo_plugins/gazebo_ros_physics_utils.h>

namespace gazebo {
TireModel::TireModel() {}

TireModel::~TireModel() {
  alive_ = false;
}

void TireModel::Load(physics::ModelPtr parent, sdf::ElementPtr sdf) {
  this->parent_ = parent;
  gazebo_ros_ = GazeboRosPtr(new GazeboRos(parent, sdf, "TireModel"));
  gazebo_ros_->isInitialized();

  gazebo_ros_->getParameter<std::string>(tireLinkName_, "tireLink",
                                         "tire_link");
  gazebo_ros_->getParameter<std::string>(baseLinkName_, "baseLink",
                                         "base_link");
  gazebo_ros_->getParameter<double>(update_rate_, "updateRate", 100.0);
  gazebo_ros_->getParameter<double>(UNLOADED_RADIUS_, "UNLOADED_RADIUS", 0.312);
  gazebo_ros_->getParameter<double>(FNOMIN_, "FNOMIN", 700.0);
  gazebo_ros_->getParameter<double>(LFZ0_, "LFZ0", 1.0);
  gazebo_ros_->getParameter<double>(PDX1_, "PDX1", 1.13);
  gazebo_ros_->getParameter<double>(PDX2_, "PDX2", -0.119);
  gazebo_ros_->getParameter<double>(PDX3_, "PDX3", -0.0000224);
  gazebo_ros_->getParameter<double>(LGAX_, "LGAX", 1.0);
  gazebo_ros_->getParameter<double>(LMUX_, "LMUX", 1.0);
  gazebo_ros_->getParameter<double>(LCX_, "LCX", 1.0);
  gazebo_ros_->getParameter<double>(PCX1_, "PCX1", 1.8);
  gazebo_ros_->getParameter<double>(LHX_, "LHX", 1.0);
  gazebo_ros_->getParameter<double>(PHX1_, "PHX1", 0.0);
  gazebo_ros_->getParameter<double>(PHX2_, "PHX2", 0.0);
  gazebo_ros_->getParameter<double>(LVX_, "LVX", 1.0);
  gazebo_ros_->getParameter<double>(PKX1_, "PKX1", 18.8);
  gazebo_ros_->getParameter<double>(PKX2_, "PKX2", -3.9);
  gazebo_ros_->getParameter<double>(PKX3_, "PKX3", 0.21);
  gazebo_ros_->getParameter<double>(LKX_, "LKX", 1.0);
  gazebo_ros_->getParameter<double>(PEX1_, "PEX1", 0.627);
  gazebo_ros_->getParameter<double>(PEX2_, "PEX2", -0.0123);
  gazebo_ros_->getParameter<double>(PEX3_, "PEX3", -0.034);
  gazebo_ros_->getParameter<double>(PEX4_, "PEX4", -0.000015);
  gazebo_ros_->getParameter<double>(LEX_, "LEX", 1.0);
  gazebo_ros_->getParameter<double>(PVX1_, "PVX1", 0.0);
  gazebo_ros_->getParameter<double>(PVX2_, "PVX2", 0.0);
  gazebo_ros_->getParameter<double>(QSY1_, "QSY1", 0.01);
  gazebo_ros_->getParameter<double>(QSY2_, "QSY2", 0.0);
  gazebo_ros_->getParameter<double>(QSY3_, "QSY3", 0.0);
  gazebo_ros_->getParameter<double>(QSY4_, "QSY4", 0.0);
  gazebo_ros_->getParameter<double>(LONGVL_, "LONGVL", 16.6);
  gazebo_ros_->getParameter<double>(KPUMIN_, "KPUMIN", -1.5);
  gazebo_ros_->getParameter<double>(KPUMAX_, "KPUMAX", 1.5);
  gazebo_ros_->getParameter<double>(FZMIN_, "FZMIN", 230.0);
  gazebo_ros_->getParameter<double>(FZMAX_, "FZMAX", 8500.0);

  // Slip Angle Range
  gazebo_ros_->getParameter<double>(ALPMIN_, "ALPMIN", -1.6);
  gazebo_ros_->getParameter<double>(ALPMAX_, "ALPMAX", 1.6);

  // Lateral Coefficients
  gazebo_ros_->getParameter<double>(PCY1_, "PCY1", 1.3);
  gazebo_ros_->getParameter<double>(PDY1_, "PDY1", 1.01);
  gazebo_ros_->getParameter<double>(PDY2_, "PDY2", -0.12);
  gazebo_ros_->getParameter<double>(PDY3_, "PDY3", -1.04);
  gazebo_ros_->getParameter<double>(PEY1_, "PEY1", -0.63);
  gazebo_ros_->getParameter<double>(PEY2_, "PEY2", -0.05);
  gazebo_ros_->getParameter<double>(PEY3_, "PEY3", -0.27);
  gazebo_ros_->getParameter<double>(PEY4_, "PEY4", -8.3);
  gazebo_ros_->getParameter<double>(PKY1_, "PKY1", -19.7);
  gazebo_ros_->getParameter<double>(PKY2_, "PKY2", 1.8);
  gazebo_ros_->getParameter<double>(PKY3_, "PKY3", 0.009);
  gazebo_ros_->getParameter<double>(PHY1_, "PHY1", 0.0);
  gazebo_ros_->getParameter<double>(PHY2_, "PHY2", 0.0);
  gazebo_ros_->getParameter<double>(PHY3_, "PHY3", -0.02);
  gazebo_ros_->getParameter<double>(PVY1_, "PVY1", 0.0);
  gazebo_ros_->getParameter<double>(PVY2_, "PVY2", 0.0);
  gazebo_ros_->getParameter<double>(PVY3_, "PVY3", -0.09);
  gazebo_ros_->getParameter<double>(PVY4_, "PVY4", -0.27);

  // Scaling Coefficients
  gazebo_ros_->getParameter<double>(LCY_, "LCY", 1.0);
  gazebo_ros_->getParameter<double>(LMUY_, "LMUY", 1.0);
  gazebo_ros_->getParameter<double>(LEY_, "LEY", 1.0);
  gazebo_ros_->getParameter<double>(LKY_, "LKY", 1.0);
  gazebo_ros_->getParameter<double>(LHY_, "LHY", 1.0);
  gazebo_ros_->getParameter<double>(LVY_, "LVY", 1.0);
  gazebo_ros_->getParameter<double>(LGAY_, "LGAY", 1.0);
  gazebo_ros_->getParameter<double>(LYKA_, "LYKA", 1.0);
  gazebo_ros_->getParameter<double>(LVYKA_, "LVYKA", 1.0);
  gazebo_ros_->getParameter<double>(LXAL_, "LXAL", 1.0);
  gazebo_ros_->getParameter<double>(LGAZ_, "LGAZ", 1.0);
  gazebo_ros_->getParameter<double>(LTR_, "LTR", 1.0);
  gazebo_ros_->getParameter<double>(LS_, "LS", 1.0);
  gazebo_ros_->getParameter<double>(LRES_, "LRES", 0.0);

  // Combined Fy Coefficients
  gazebo_ros_->getParameter<double>(RBY1_, "RBY1", 6.23);
  gazebo_ros_->getParameter<double>(RBY2_, "RBY2", 3.07);
  gazebo_ros_->getParameter<double>(RBY3_, "RBY3", 0.016);
  gazebo_ros_->getParameter<double>(RCY1_, "RCY1", 1.005);
  gazebo_ros_->getParameter<double>(REY1_, "REY1", 0.019);
  gazebo_ros_->getParameter<double>(REY2_, "REY2", -0.002);
  gazebo_ros_->getParameter<double>(RHY1_, "RHY1", 0.0);
  gazebo_ros_->getParameter<double>(RHY2_, "RHY2", 0.0);
  gazebo_ros_->getParameter<double>(RVY1_, "RVY1", 0.0);
  gazebo_ros_->getParameter<double>(RVY2_, "RVY2", 0.0);
  gazebo_ros_->getParameter<double>(RVY3_, "RVY3", 0.0);
  gazebo_ros_->getParameter<double>(RVY4_, "RVY4", 0.0);
  gazebo_ros_->getParameter<double>(RVY5_, "RVY5", 0.0);
  gazebo_ros_->getParameter<double>(RVY6_, "RVY6", 0.0);

  // Combined Fx Coefficients
  gazebo_ros_->getParameter<double>(RBX1_, "RBX1", 5.99);
  gazebo_ros_->getParameter<double>(RBX2_, "RBX2", -8.26);
  gazebo_ros_->getParameter<double>(RCX1_, "RCX1", 1.078);
  gazebo_ros_->getParameter<double>(REX1_, "REX1", 1.644);
  gazebo_ros_->getParameter<double>(REX2_, "REX2", -0.006);
  gazebo_ros_->getParameter<double>(RHX1_, "RHX1", 0.008);

  // Aligning Torque Coefficients
  gazebo_ros_->getParameter<double>(QHZ1_, "QHZ1", 0.0);
  gazebo_ros_->getParameter<double>(QHZ2_, "QHZ2", 0.0);
  gazebo_ros_->getParameter<double>(QHZ3_, "QHZ3", 0.06);
  gazebo_ros_->getParameter<double>(QHZ4_, "QHZ4", 0.11);
  gazebo_ros_->getParameter<double>(QDZ1_, "QDZ1", 0.06);
  gazebo_ros_->getParameter<double>(QDZ2_, "QDZ2", 0.0005);
  gazebo_ros_->getParameter<double>(QDZ3_, "QDZ3", -0.6);
  gazebo_ros_->getParameter<double>(QDZ4_, "QDZ4", 8.6);
  gazebo_ros_->getParameter<double>(QDZ6_, "QDZ6", -0.004);
  gazebo_ros_->getParameter<double>(QDZ7_, "QDZ7", 0.003);
  gazebo_ros_->getParameter<double>(QDZ8_, "QDZ8", -0.11);
  gazebo_ros_->getParameter<double>(QDZ9_, "QDZ9", 0.021);
  gazebo_ros_->getParameter<double>(QBZ1_, "QBZ1", 7.5);
  gazebo_ros_->getParameter<double>(QBZ2_, "QBZ2", -1.9);
  gazebo_ros_->getParameter<double>(QBZ3_, "QBZ3", 0.61);
  gazebo_ros_->getParameter<double>(QBZ4_, "QBZ4", 0.12);
  gazebo_ros_->getParameter<double>(QBZ5_, "QBZ5", 0.5);
  gazebo_ros_->getParameter<double>(QBZ9_, "QBZ9", 5.51);
  gazebo_ros_->getParameter<double>(QBZ10_, "QBZ10", 0.0);
  gazebo_ros_->getParameter<double>(QCZ1_, "QCZ1", 1.2);
  gazebo_ros_->getParameter<double>(QEZ1_, "QEZ1", -5.4);
  gazebo_ros_->getParameter<double>(QEZ2_, "QEZ2", 1.1);
  gazebo_ros_->getParameter<double>(QEZ3_, "QEZ3", 0.0);
  gazebo_ros_->getParameter<double>(QEZ4_, "QEZ4", 0.2);
  gazebo_ros_->getParameter<double>(QEZ5_, "QEZ5", -3.0);
  gazebo_ros_->getParameter<double>(SSZ1_, "SSZ1", 0.02);
  gazebo_ros_->getParameter<double>(SSZ2_, "SSZ2", 0.02);
  gazebo_ros_->getParameter<double>(SSZ3_, "SSZ3", 0.001);
  gazebo_ros_->getParameter<double>(SSZ4_, "SSZ4", 0.001);

  gazebo_ros_->getParameter<double>(CC1_, "CC1", 0.0);
  gazebo_ros_->getParameter<double>(CC2_, "CC2", 0.0);
  gazebo_ros_->getParameter<double>(CC3_, "CC3", 0.0);
  gazebo_ros_->getParameter<double>(CC4_, "CC4", 0.0);

  wheelCollides_ = false;

  FZ0T_ = FNOMIN_ * LFZ0_;
  CX_ = PCX1_ * LCX_;
  if (CX_ <= 0) {
    ROS_ERROR("CX should be > 0");
  }
  CY_ = PCY1_ * LCY_;
  if (CY_ <= 0) {
    ROS_ERROR("CY should be > 0");
  }
  tireJoint_ = gazebo_ros_->getJoint(parent_, "tireJoint", "tireJoint");

  tireJoint_->SetProvideFeedback(true);

  tireLink_ = parent_->GetLink(tireLinkName_);
  carLink_ = parent_->GetLink(baseLinkName_);
  anchorPose_ = tireJoint_->GetChild()->InitialRelativePose().Pos();
  if (this->update_rate_ > 0.0) {
    this->update_period_ = 1.0 / this->update_rate_;
  } else {
    this->update_period_ = 0.0;
  }

  alive_ = true;

  this->update_connection_ = event::Events::ConnectWorldUpdateBegin(
      boost::bind(&TireModel::UpdateChild, this));
  std::vector<std::string> collisions;
  for (auto &x : tireLink_->GetCollisions()) {
    collisions.push_back(x->GetScopedName());
  }

  physics::ContactManager *mgr =
      parent->GetWorld()->Physics()->GetContactManager();
  std::string topic = mgr->CreateFilter(tireLink_->GetScopedName(), collisions);
  transport::NodePtr node(new transport::Node());
  node->Init("default");
  if (!this->contactSub_) {
    this->contactSub_ = node->Subscribe(topic, &TireModel::OnContacts, this);
  }
}

void TireModel::Init() { gazebo::ModelPlugin::Init(); }

void TireModel::Reset() {
  gazebo::ModelPlugin::Reset();
  last_update_time_ = parent_->GetWorld()->SimTime();
  this->contactSub_.reset();
}

void TireModel::OnContacts(ConstContactsPtr &contactsMsg) {
  wheelCollides_ = contactsMsg->contact_size() > 0;
}

double TireModel::GetCamberFromToeAngle(double angle) {
  return CC1_ + CC2_ * angle + CC3_ * (angle * angle) + CC4_ * (angle * angle * angle);
}

void TireModel::UpdateChild() {
  common::Time current_time = parent_->GetWorld()->SimTime();
  double elapsedTime = (current_time - last_update_time_).Double();
  last_update_time_ = current_time;
  double Fz = 0 - tireJoint_->GetForceTorque(0).body1Force.Z();
  ignition::math::Vector3d linearVehicleVelocity = parent_->RelativeLinearVel();
  ignition::math::Vector3d angularVehicleVelocity =
      parent_->RelativeAngularVel();
  if (!ignition::math::isnan(Fz) && wheelCollides_ &&
      !ignition::math::isnan(angularVehicleVelocity.Z()) &&
      !ignition::math::isnan(linearVehicleVelocity.X()) &&
      !ignition::math::isnan(linearVehicleVelocity.Y()) &&
      !ignition::math::isnan(linearVehicleVelocity.Z())) {
    Fz = ignition::math::clamp(Fz, FZMIN_, FZMAX_);
    double wheelVelocity = tireJoint_->GetVelocity(0);

    angularVehicleVelocity.X(0);
    angularVehicleVelocity.Y(0);

    ignition::math::Vector3d angularVelCrossAnchorPose =
        angularVehicleVelocity.Cross(anchorPose_);
    ignition::math::Quaternion<double> tireRotation =
        tireJoint_->GetChild()->RelativePose().Rot();
    double toeAngle = -tireRotation.Euler().Z();
    if (fabs(toeAngle) < 0.0001) { toeAngle = 0; } //gazebo reports -0.000001
    if (toeAngle > ( M_PI / 2.0)) { toeAngle = toeAngle - M_PI; }
    if (toeAngle < (-M_PI / 2.0)) { toeAngle = M_PI + toeAngle; }
    ignition::math::Vector3d vb1 =
        linearVehicleVelocity + angularVelCrossAnchorPose;
    ignition::math::Vector3d velocityTireFrame(
        vb1.X() * cos(toeAngle) + vb1.Y() * -sin(toeAngle),
        vb1.X() * sin(toeAngle) + vb1.Y() * cos(toeAngle), 0);

    double dFz = (Fz - FZ0T_) / FZ0T_;

    double slip = GetSlip(velocityTireFrame.X(), wheelVelocity);
    
    double slipAngle =
      velocityTireFrame.Y() / fabs(velocityTireFrame.X());
      //TODO epsilon
    // slipAngle = ignition::math::clamp(slipAngle, ALPMIN_, ALPMAX_);

    
    double camber = GetCamberFromToeAngle(-toeAngle);
    double Fx = GetCombinedFx(slipAngle, slip, Fz, dFz, camber);
    double Fy = GetCombinedFy(slipAngle, slip, Fz, dFz, camber);
    ignition::math::Vector3d torque(
        0, 0, GetSelfAligningTorque(slipAngle, dFz, camber, slip, Fz, Fy, Fx));
    ignition::math::Vector3d tireFrameForce(Fx, Fy, 0);
    ignition::math::Vector3d carFrameForce;
    carFrameForce =
        ignition::math::Vector3d(tireFrameForce.X() * cos(-toeAngle) +
                                      tireFrameForce.Y() * -sin(-toeAngle),
                                  tireFrameForce.X() * sin(-toeAngle) +
                                      tireFrameForce.Y() * cos(-toeAngle),
                                  0);
    if (!ignition::math::isnan(carFrameForce.X()) &&
        !ignition::math::isnan(carFrameForce.Y())) {  // TODO why nan at start?
      carLink_->AddLinkForce(carFrameForce, anchorPose_);
      carLink_->AddTorque(torque);
      double opposingtorque = -tireFrameForce.X() * UNLOADED_RADIUS_;

      double rollingResistance =
          GetRollingResistance(Fz, velocityTireFrame.X(), tireFrameForce.X()) * physics_utils::fsgn(wheelVelocity);
      rollingResistance = physics_utils::limit_brake_torque(wheelVelocity, tireLink_->GetInertial()->IYY(), 
      rollingResistance, -opposingtorque, elapsedTime);
      opposingtorque += rollingResistance;
      tireJoint_->SetForce(0, opposingtorque);
    }
  }
}

double TireModel::GetSlip(double velocityTireFrameX, double wheelVelocity) {
  if (0 == velocityTireFrameX || ignition::math::isnan(velocityTireFrameX)) {
    const double epsilon = 0.01;
    velocityTireFrameX = epsilon; //this small epsilon is suggested to prevent singularity
  }
  if (ignition::math::isnan(wheelVelocity)) {
    wheelVelocity = 0;
  }
  // TODO radius here should be effective tire radius
  double effectiveTireRadius = UNLOADED_RADIUS_;
  double Vsx = velocityTireFrameX - wheelVelocity * effectiveTireRadius;
  double slip = (-Vsx) / fabs(velocityTireFrameX);
  slip = ignition::math::clamp(slip, KPUMIN_, KPUMAX_);
  return slip;
}

double TireModel::GetFx0(double slip, double Fz, double dFz, double camber) {
  double camberX = camber * LGAX_;
  double muX =
      (PDX1_ + PDX2_ * dFz) * (1 - PDX3_ * (camberX * camberX)) * LMUX_;
  double Dx = muX * Fz;
  if (Dx <= 0) {
    ROS_ERROR("Dx must be > 0");
  }
  double SHx = (PHX1_ + PHX2_ * dFz) * LHX_;
  double SVx = Fz * (PVX1_ + PVX2_ * dFz) * LVX_ * LMUX_;
  double kappax = slip + SHx;
  double Kxkappa = Fz * (PKX1_ + PKX2_ * dFz) * exp(PKX3_ * dFz) * LKX_;
  const double epsilonX = 0.01; 
  double Bx = Kxkappa / (CX_ * Dx + epsilonX);
  double BxKappax = Bx * kappax;
  double Ex = (PEX1_ + PEX2_ * dFz + PEX3_ * (dFz * dFz)) *
              (1 - PEX4_ * physics_utils::fsgn(kappax)) * LEX_;
  Ex = fmin(1, Ex);
  double Fx0 =
      Dx * sin(CX_ * atan(BxKappax - Ex * (BxKappax - atan(BxKappax)))) + SVx;
  Kxkappa_ = Kxkappa;
  return Fx0;
}

double TireModel::GetFy0(double slipAngle, double Fz, double dFz,
                         double camber) {
  double camberY = camber * LGAY_;
  double muY = (PDY1_ + PDY2_ * dFz) * (1 - PDY3_ * camberY * camberY) * LMUY_;
  double Dy = muY * Fz;
  double SHy = (PHY1_ + PHY2_ * dFz) * LHY_ + PHY3_ * camberY;
  double SVy =
      Fz * ((PVY1_ + PVY2_ * dFz) * LVY_ + (PVY3_ + PVY4_ * dFz) * camberY) *
      LMUY_;
  double alphay = slipAngle + SHy;
  double Kyalpha = PKY1_ * FZ0T_ * (1 - PKY3_ * fabs(camberY)) *
              sin(2 * atan(Fz / (PKY2_ * FZ0T_))) * LKY_;
  const double epsilonY = 0.01;
  double By = Kyalpha / (CY_ * Dy + epsilonY);
  double ByAlphay = By * alphay;
  double Ey = (PEY1_ + PEY2_ * dFz) *
              (1 - (PEY3_ + PEY4_ * camberY) * physics_utils::fsgn(alphay)) * LEY_;
  Ey = fmin(1,Ey);
  double Fy0 =
      Dy * sin(CY_ * atan(ByAlphay - Ey * (ByAlphay - atan(ByAlphay)))) + SVy;
  Kyalpha_ = Kyalpha;
  By_ = By;
  SHy_ = SHy;
  SVy_ = SVy;
  MuY_ = muY;
  return Fy0;
}

double TireModel::GetGyk(double slipAngle, double slip, double dFz) {
    double Byk = RBY1_ * cos(atan(RBY2_ * (slipAngle - RBY3_))) * LYKA_;
    double SHyk = RHY1_ + RHY2_ * dFz;
    double Eyk = REY1_ + REY2_ * dFz;
    Eyk = fmin(1, Eyk);
    double BykShyk = Byk * SHyk;
    double ks = slip + SHyk;
    double Bykks = Byk * ks;
    double Gyk = cos(RCY1_ * atan(Bykks - Eyk * (Bykks - atan(Bykks)))) /
               cos(RCY1_ * atan(BykShyk - Eyk * (BykShyk - atan(BykShyk))));
    return Gyk;
}

double TireModel::GetCombinedFy(double slipAngle, double slip, double Fz,
                                double dFz, double camber) {
  double fy0 = GetFy0(slipAngle, Fz, dFz, camber);
  double DVyk = MuY_ * Fz * (RVY1_ + RVY2_ * dFz + RVY3_ * camber) *
                cos(atan(RVY4_ * slipAngle));
  double SVyk = DVyk * sin(RVY5_ * atan(RVY6_ * slip)) * LVYKA_;
  double fy = fy0 * GetGyk(slipAngle, slip, dFz) + SVyk;
  return fy;
}

double TireModel::GetCombinedFx(double slipAngle, double slip, double Fz, double dFz,
                                double camber) {
  double fx0 = GetFx0(slip, Fz, dFz, camber);
  double Exa = REX1_ + REX2_ * dFz;
  Exa = fmin (1, Exa);
  double Bxa = RBX1_ * cos(atan(RBX2_ * slip)) * LXAL_;
  if (Bxa <= 0) {
    ROS_ERROR("Bxa should be > 0");
  }
  double as = slipAngle + RHX1_;
  double Bxaas = Bxa * as;
  double BxaShxa = Bxa * RHX1_;
  double Gxa = cos(RCX1_ * atan(Bxaas - Exa * (Bxaas - atan(Bxaas)))) /
               cos(RCX1_ * atan(BxaShxa - Exa * (BxaShxa - atan(BxaShxa))));
  return fx0 * Gxa;
}

double TireModel::GetSelfAligningTorque(double slipAngle, double dFz,
                                        double camber, double slip, double Fz,
                                        double Fy, double Fx) {
  double camberz = camber * LGAZ_;
  double SHt = QHZ1_ + QHZ2_ * dFz + (QHZ3_ + QHZ4_ * dFz) * camberz;
  double alphat = slipAngle + SHt;
  const double epsilonK = 0.01;
  double KyalphaT = Kyalpha_ + epsilonK;
  double KxKy = (Kxkappa_ / KyalphaT);
  double KxKyKxKyslipslip = KxKy * KxKy * slip * slip;
  double tanalphat = tan(alphat);
  double alphateq =
      atan(sqrt(tanalphat * tanalphat + KxKyKxKyslipslip)) * physics_utils::fsgn(alphat);
  double SHf = SHy_ + SVy_ / KyalphaT;
  double alphar = slipAngle + SHf;
  double tanalphar = tan(alphar);
  double camberzcamberz = camberz * camberz;
  double alphareq =
      atan(sqrt(tanalphar * tanalphar + KxKyKxKyslipslip)) * physics_utils::fsgn(alphar);
  double Dt0 = Fz * (UNLOADED_RADIUS_ / FNOMIN_) * (QDZ1_ + QDZ2_ * dFz) * LTR_;
  double Dt =  Dt0 *
              (1 + QDZ3_ * camberz + QDZ4_ * camberzcamberz);
  double Bt = (QBZ1_ + QBZ2_ * dFz + QBZ3_ * dFz * dFz) *
              (1 + QBZ4_ * camberz + QBZ5_ * fabs(camberz)) *
              (LKY_ / LMUY_);
  double Et = (QEZ1_ + QEZ2_ * dFz + QEZ3_ * dFz * dFz) * 
  (1 + (QEZ4_ + QEZ5_* camberz) * (2.0/M_PI)*atan(Bt*QCZ1_*alphat));
  Et = fmin(1, Et);
  double Btalphateq = Bt * alphateq;
  double t = Dt * cos(QCZ1_ *
                      atan(Btalphateq - Et * (Btalphateq - atan(Btalphateq)))) *
             cos(slipAngle);
  double Dr = Fz *
              ((QDZ6_ + QDZ7_ * dFz) * LRES_ + (QDZ8_ + QDZ9_ * dFz) * camberz) *
              UNLOADED_RADIUS_ * LMUY_;
  double Br = QBZ9_ * (LKY_ / LMUY_) + QBZ10_ * By_ * CY_;
  double Mzr = Dr * cos(atan(Br * alphareq)) * cos(slipAngle);
  double s = (SSZ1_ + SSZ2_ * (Fy / FZ0T_) + (SSZ3_ + SSZ4_ * dFz) * camber) *
             UNLOADED_RADIUS_ * LS_;
  double FyT = GetGyk(slipAngle, slip, dFz) * GetFy0(slipAngle,
              Fz, dFz, 0);
  double Mz = -t * FyT + Mzr + s * Fx;
  return Mz;
}

double TireModel::GetRollingResistance(double Fz, double Vx,
                                       double Fx) {
  double My =
      UNLOADED_RADIUS_ * Fz * (QSY1_ + QSY2_ * Fx / FNOMIN_ + QSY3_ * fabs(Vx / LONGVL_) +
                      pow(QSY4_ * (Vx / LONGVL_), 4));
  return My;
}

void TireModel::FiniChild() {
  alive_ = false;
  gazebo_ros_->node()->shutdown();
}

GZ_REGISTER_MODEL_PLUGIN(TireModel)
}
