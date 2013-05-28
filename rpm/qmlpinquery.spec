# 
# Do NOT Edit the Auto-generated Part!
# Generated by: spectacle version 0.26
# 

Name:       qmlpinquery

# >> macros
# << macros

Summary:    QML based PIN query application using ofono-qt
Version:    0.1.3
Release:    1
Group:      Applications/Communications
License:    BSD
URL:        https://github.com/nemomobile/qmlpinquery
Source0:    %{name}-%{version}.tar.bz2
Source1:    qmlpinquery.service
Source100:  qmlpinquery.yaml
Requires:   qt-components
BuildRequires:  pkgconfig(qdeclarative-boostable)
BuildRequires:  pkgconfig(QtCore) >= 4.7.0
BuildRequires:  pkgconfig(QtGui)
BuildRequires:  pkgconfig(QtDBus)
BuildRequires:  pkgconfig(QtDeclarative)
BuildRequires:  pkgconfig(ofono-qt)
Provides:   meego-pinquery > 0.0.2
Obsoletes:   meego-pinquery <= 0.0.2

%description
QML PIN query dialog

%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qmake 

make %{?jobs:-j%jobs}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake_install
mkdir -p %{buildroot}%{_libdir}/systemd/user/
cp -a %{SOURCE1} %{buildroot}%{_libdir}/systemd/user/


# >> install post
# << install post

%files
%defattr(-,root,root,-)
%{_bindir}/qmlpinquery
%{_usr}/share/applications/qmlpinquery.desktop
%{_libdir}/systemd/user/qmlpinquery.service
# >> files
# << files
